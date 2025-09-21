# Data Structures and Memory Management

## Core Data Types

### Primary Structures

#### `t_data` - Main Application Container
```c
typedef struct s_data {
    t_gl        *gl;         // Graphics rendering context
    t_fract     *fract;      // Fractal computation parameters  
    float3      *vertexpos;  // Vertex positions for marching cubes
    float       *vertexval;  // Scalar field values at vertices
    float3      **triangles; // Generated triangle mesh
    uint2       len;         // Array size tracking
} t_data;
```

**Memory Allocation**:
- `vertexpos`: `grid_size³ × 8 × sizeof(float3)` 
- `vertexval`: `grid_size³ × 8 × sizeof(float)`
- `triangles`: Dynamic allocation based on surface complexity

#### `t_fract` - Fractal Parameters
```c
typedef struct s_fract {
    float3      p0, p1;      // Bounding box corners (-1.5 to 1.5)
    float       step_size;   // Grid resolution (default: 0.05)
    float       grid_length; // Total dimension (3.0)
    float       grid_size;   // Computed: grid_length/step_size
    t_julia     *julia;      // Julia set parameters
    t_grid      grid;        // 3D coordinate arrays
    t_voxel     voxel[8];    // Cube corner offsets
} t_fract;
```

#### `t_julia` - Julia Set Configuration
```c
typedef struct s_julia {
    uint        max_iter;    // Iteration limit (default: 6)
    float       threshold;   // Escape radius (default: 2.0)
    float       w;           // 4th dimension value (default: 0.0)
    cl_quat     c;           // Quaternion constant
} t_julia;
```

**Default Values**:
```c
julia->c.x = -0.2f;  // Real part
julia->c.y = 0.8f;   // i component  
julia->c.z = 0.0f;   // j component
julia->c.w = 0.0f;   // k component
```

### Graphics Structures

#### `t_gl` - OpenGL Context
```c
typedef struct s_gl {
    GLFWwindow  *window;     // GLFW window handle
    int         export;      // Export flag for OBJ saving
    GLuint      vertexShader, fragmentShader, shaderProgram;
    GLuint      vbo, vao;    // OpenGL buffer objects
    float       *tris;       // Flattened triangle data
    uint        num_pts;     // Total vertex count
    uint        num_tris;    // Triangle count
    t_matrix    *matrix;     // Transformation matrices
} t_gl;
```

#### `t_matrix` - Transformation Matrices
```c
typedef struct s_matrix {
    mat4        model_mat;      // Object transformation
    mat4        projection_mat; // Perspective projection
    mat4        view_mat;       // Camera transformation
    vec3        eye, center, up; // Camera parameters
    GLuint      model, view;    // Uniform locations
} t_matrix;
```

### Utility Structures

#### `t_grid` - 3D Coordinate Arrays
```c
typedef struct s_grid {
    float       *x;  // X-axis subdivisions
    float       *y;  // Y-axis subdivisions  
    float       *z;  // Z-axis subdivisions
} t_grid;
```

#### `t_voxel` - Cube Corner Offsets
```c
typedef struct s_voxel {
    float       dx, dy, dz;  // Offset from cube center
} t_voxel;
```

**Voxel Configuration** (8 corners of a cube):
```
Corner Layout:
    4-------5
   /|      /|
  / |     / |
 0-------1  |
 |  7----|--6
 | /     | /
 |/      |/
 3-------2
```

## Memory Management Patterns

### Allocation Strategy

#### Dynamic Triangle Storage
```c
float3 **alloc_float3_arr(float3 **mem, uint2 *len) {
    uint size = len->x + len->y;
    if (!mem) {
        mem = malloc(sizeof(float3 *) * size);
        for (uint c = len->x; c < size; c++) {
            mem[c] = malloc(3 * sizeof(float3));
        }
    } else {
        mem = realloc(mem, sizeof(float3 *) * size);
        for (uint c = len->x; c < size; c++) {
            mem[c] = malloc(3 * sizeof(float3));
        }
    }
    return mem;
}
```

#### Array Concatenation
```c
float3 **arr_float3_cat(float3 **f_from, float3 **f_to, uint2 *len) {
    uint res_c = len->x + len->y;
    f_to = alloc_float3_arr(f_to, len);
    
    // Copy data from source to destination
    uint from_c = 0, to_c = len->x;
    while (to_c < res_c) {
        f_to[to_c][0] = f_from[from_c][0];
        f_to[to_c][1] = f_from[from_c][1]; 
        f_to[to_c][2] = f_from[from_c][2];
        to_c++; from_c++;
    }
    
    // Clean up source array
    if (f_from) {
        for (uint i = 0; i < len->y; i++) free(f_from[i]);
        free(f_from);
    }
    
    len->x = res_c;
    return f_to;
}
```

### Cleanup Procedures

#### Complete Data Cleanup
```c
void clean_up(t_data *data) {
    if (data) {
        if (data->gl) clean_gl(data->gl);
        if (data->fract) clean_fract(data->fract);
        if (data->vertexpos) free(data->vertexpos);
        if (data->vertexval) free(data->vertexval);
        if (data->triangles) clean_trigs(data->triangles, data->len.x);
        free(data);
    }
}
```

#### Graphics Cleanup
```c
void clean_gl(t_gl *gl) {
    if (gl->matrix) free(gl->matrix);
    if (gl->tris) free(gl->tris);
    free(gl);
}

void terminate_gl(t_gl *gl) {
    glDeleteVertexArrays(1, &gl->vao);
    glDeleteBuffers(1, &gl->vbo);
    glDeleteProgram(gl->shaderProgram);
    glfwTerminate();
}
```

## Type Definitions and Dependencies

### OpenCL Vector Types
The system relies on OpenCL vector types that may not be available:

```c
// From lib_complex.h - PROBLEMATIC
#include <OpenCL/opencl.h>
#include "opencl-c-base.h"

#if defined(DOUBLE_SUPPORT_AVAILABLE)
    typedef double2   cl_complex;
    typedef cl_double4 cl_quat; 
    typedef double    TYPE;
#else
    typedef float2    cl_complex;  // ❌ May not be defined
    typedef float4    cl_quat;     // ❌ May not be defined
    typedef float     TYPE;
#endif
```

### Missing Type Definitions
The following types are used but may not be properly defined:
- `float2`, `float3`, `float4` - OpenCL vector types
- `uint2` - 2-component unsigned integer vector
- `mat4`, `vec3` - cglm matrix/vector types

## Memory Usage Analysis

### Grid Memory Requirements
For a typical configuration:
- **Step Size**: 0.05
- **Grid Size**: 3.0 / 0.05 = 60
- **Total Vertices**: 60³ × 8 = 1,728,000 vertices
- **Memory Usage**: 
  - Positions: 1.7M × 12 bytes = ~20MB
  - Values: 1.7M × 4 bytes = ~7MB
  - **Total**: ~27MB for computation arrays

### Triangle Storage
- **Variable Size**: Depends on fractal complexity
- **Worst Case**: Could generate millions of triangles
- **Dynamic Growth**: Uses realloc for expansion

### OpenGL Buffer Memory
- **Vertex Buffer**: `num_pts × sizeof(float)` 
- **Typical Size**: Hundreds of MB for complex fractals
- **GPU Memory**: Additional copy stored on graphics card

## Error Handling and Safety

### Memory Allocation Checking
```c
if (!(data = (t_data *)malloc(sizeof(t_data))))
    error(MALLOC_FAIL_ERR, NULL);
```

### Error Propagation
```c
void error(int errno, t_data *data) {
    // Print appropriate error message
    clean_up(data);  // Ensure cleanup before exit
    exit(1);
}
```

### Resource Management Issues
1. **Missing NULL checks** in some allocation paths
2. **Inconsistent cleanup** in error conditions  
3. **Potential memory leaks** in triangle generation
4. **OpenGL resource cleanup** may be incomplete

## Optimization Opportunities

### Memory Efficiency
1. **Reduce Temporary Storage**: Eliminate intermediate arrays where possible
2. **Streaming Processing**: Process data in chunks rather than loading entire volume
3. **Memory Pool**: Pre-allocate common sizes to reduce fragmentation

### Cache Optimization
1. **Data Locality**: Reorganize structures for better cache usage
2. **Loop Optimization**: Improve iteration patterns for cache efficiency
3. **Prefetching**: Add hints for memory access patterns

### GPU Memory Management
1. **Buffer Reuse**: Avoid recreating buffers for each frame
2. **Texture Optimization**: Efficient storage of fractal data
3. **Streaming**: Upload data incrementally for large datasets
