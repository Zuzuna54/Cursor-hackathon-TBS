# System Architecture

## High-Level Overview

Morphosis follows a modular architecture with clear separation between mathematical computation, data management, and graphics rendering:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Input Layer   │───▶│ Computation     │───▶│ Rendering       │
│                 │    │ Engine          │    │ Pipeline        │
│ • CLI Arguments │    │ • Julia Set     │    │ • OpenGL        │
│ • Matrix Files  │    │ • Marching      │    │ • Shaders       │
│ • Poem Files    │    │   Cubes         │    │ • Buffers       │
│ • Default Vals  │    │ • Polygonization│    │ • Export        │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## Core Data Flow

1. **Initialization Phase**
   - Parse command line arguments
   - Initialize data structures (`t_data`, `t_gl`, `t_fract`, `t_julia`)
   - Set up fractal parameters and grid configuration

2. **Computation Phase**
   - Generate 3D spatial grid based on step size
   - Sample 4D Julia set at each grid point
   - Apply marching cubes algorithm for surface extraction
   - Generate triangle mesh from implicit surface

3. **Rendering Phase**
   - Convert triangles to OpenGL vertex buffers
   - Initialize OpenGL context and shaders
   - Render with real-time rotation and interaction
   - Optional: Export mesh to OBJ file format

## Main Data Structures

### `t_data` - Central Data Container
```c
typedef struct s_data {
    t_gl        *gl;         // Graphics context and rendering data
    t_fract     *fract;      // Fractal generation parameters
    float3      *vertexpos;  // Vertex positions for computation
    float       *vertexval;  // Scalar field values at vertices
    float3      **triangles; // Generated triangle mesh
    uint2       len;         // Array lengths and counters
} t_data;
```

### `t_fract` - Fractal Configuration
```c
typedef struct s_fract {
    float3      p0, p1;      // Bounding box corners
    float       step_size;   // Grid resolution
    float       grid_length; // Total grid dimension
    float       grid_size;   // Number of grid points
    t_julia     *julia;      // Julia set parameters
    t_grid      grid;        // 3D coordinate arrays
    t_voxel     voxel[8];    // Cube corner offsets
} t_fract;
```

### `t_julia` - Julia Set Parameters
```c
typedef struct s_julia {
    uint        max_iter;    // Maximum iteration count
    float       threshold;   // Escape radius (typically 2.0)
    float       w;           // 4th dimension coordinate
    cl_quat     c;           // Complex quaternion constant
} t_julia;
```

### `t_gl` - Graphics Context
```c
typedef struct s_gl {
    GLFWwindow  *window;     // GLFW window handle
    int         export;      // Export flag for OBJ saving
    GLuint      shaderProgram, vertexShader, fragmentShader;
    GLuint      vbo, vao;    // Vertex buffer objects
    float       *tris;       // Triangle vertex data
    uint        num_pts, num_tris; // Counts
    t_matrix    *matrix;     // Transformation matrices
} t_gl;
```

## Module Dependencies

```
morphosis.h (main header)
├── gl_includes.h
│   ├── GLFW/glfw3.h
│   ├── stb_image.h
│   ├── cglm/cglm.h
│   └── structures.h
├── errors.h
├── obj.h
├── matrix.h
└── lib_complex.h
    ├── OpenCL/opencl.h
    ├── opencl-c-base.h
    └── math.h
```

## File Organization

### Source Files (`srcs/`)
- **Core**: `main.c`, `init.c`, `cleanup.c`, `errors.c`, `utils.c`
- **Fractal**: `point_cloud.c`, `build_fractal.c`, `sample_julia.c`, `polygonisation.c`
- **Graphics**: `gl_*.c` files (8 modules)
- **Export**: `write_obj.c`, `obj.c`
- **Math**: `lib_complex.c`
- **Matrix**: `matrix_*.c`, `poem.c`

### Headers (`includes/`)
- **Main**: `morphosis.h`, `structures.h`
- **Graphics**: `gl_includes.h`
- **Math**: `lib_complex.h`, `matrix.h`
- **Utilities**: `errors.h`, `obj.h`, `look-up.h`
- **External**: `stb_image.h`

### Support Files
- **Shaders**: `vertex.shader`, `fragment.shader`
- **Library**: `libft/` (custom utility library)
- **Build**: `Makefile`, `CMakeLists.txt`

## Execution Flow

1. **`main()`** → Parse arguments and initialize data
2. **`calculate_point_cloud()`** → Generate fractal geometry
3. **`gl_retrieve_tris()`** → Prepare data for rendering
4. **`run_graphics()`** → Initialize OpenGL and render loop
5. **`export_obj()`** → Optional mesh export
6. **`clean_up()`** → Resource cleanup

## Key Algorithms

1. **4D Julia Set Iteration**: Quaternion-based fractal computation
2. **Marching Cubes**: Surface extraction from scalar fields
3. **Triangle Mesh Generation**: Geometric surface representation
4. **Real-time Rendering**: OpenGL-based 3D visualization
