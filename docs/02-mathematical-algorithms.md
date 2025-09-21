# Mathematical Algorithms

## 4D Julia Set Theory

### Mathematical Foundation

The 4D Julia set is defined by the iterative equation:
```
z_{n+1} = z_n² + c
```

Where:
- `z` is a quaternion representing a point in 4D space
- `c` is a quaternion constant that defines the specific Julia set
- The iteration continues until either:
  - `|z| > threshold` (point escapes, not in set)
  - `iterations > max_iter` (point assumed to be in set)

### Quaternion Mathematics

#### Quaternion Structure
```c
typedef struct {
    float x, y, z, w;  // Real and three imaginary components
} cl_quat;
```

#### Core Operations

**Quaternion Multiplication** (`cl_quat_mult`):
```c
result.x = (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) - (q1.w * q2.w);
result.y = (q1.x * q2.y) + (q1.y * q2.x) + (q1.z * q2.w) - (q1.w * q2.z);
result.z = (q1.x * q2.z) + (q1.z * q2.x) + (q1.w * q2.y) - (q1.y * q2.w);
result.w = (q1.x * q1.w) + (q1.w * q2.x) + (q1.y * q2.z) - (q1.z * q2.y);
```

**Quaternion Addition** (`cl_quat_sum`):
```c
result.x = q1.x + q2.x;
result.y = q1.y + q2.y; 
result.z = q1.z + q2.z;
result.w = q1.w + q2.w;
```

**Quaternion Magnitude** (`cl_quat_mod`):
```c
conjugate = cl_quat_conjugate(q);
product = cl_quat_mult(q, conjugate);
magnitude = sqrt(product.x² + product.y² + product.z² + product.w²);
```

### Julia Set Sampling Algorithm

The `sample_4D_Julia()` function implements the core fractal test:

```c
float sample_4D_Julia(t_julia *julia, float3 pos) {
    cl_quat z;
    uint iter = 0;
    
    // Initialize quaternion from 3D position + w component
    z.x = pos.x;
    z.y = pos.y; 
    z.z = pos.z;
    z.w = julia->w;
    
    // Iterate until escape or max iterations
    while (iter < julia->max_iter) {
        z = cl_quat_mult(z, z);      // z² 
        z = cl_quat_sum(z, julia->c); // z² + c
        
        if (cl_quat_mod(z) > 2.0f)   // Escape condition
            return 0.0f;             // Outside set
        iter++;
    }
    return 1.0f;                     // Inside set
}
```

## Marching Cubes Algorithm

### Overview
The marching cubes algorithm extracts a polygonal mesh from a 3D scalar field. It processes the volume as a grid of cubes, determining where the surface intersects each cube.

### Implementation Details

#### Grid Generation
1. **Spatial Subdivision**: 3D space divided into uniform grid
2. **Voxel Definition**: Each cube has 8 corner vertices
3. **Scalar Sampling**: Julia set function evaluated at each vertex

#### Surface Extraction Process

1. **Cube Classification**:
   ```c
   uint getCubeIndex(float *v_val, uint pos) {
       uint cubeindex = 0;
       for (int i = 0; i < 8; i++) {
           if (v_val[pos + i]) cubeindex |= (1 << i);
       }
       return cubeindex;
   }
   ```

2. **Edge Intersection Calculation**:
   ```c
   float3 interpolate(float3 p0, float3 p1, float v0, float v1) {
       if (v0 == 1.0f) return p0;
       if (v1 == 1.0f) return p1;
       if ((v1 - v0) == 0.0f) return p0;
       
       float mu = (1.0f - v0) / (v1 - v0);
       return p0 + mu * (p1 - p0);
   }
   ```

3. **Triangle Generation**: Uses lookup tables (`edgetable[]`, `tritable[][]`) to determine triangle configuration for each cube type.

### Lookup Tables

- **`edgetable[256]`**: Determines which edges are intersected for each cube configuration
- **`tritable[256][16]`**: Defines triangle vertices for each cube configuration
- **256 configurations**: All possible combinations of 8 cube vertices being inside/outside the surface

## Grid and Voxel System

### 3D Grid Generation
```c
void create_grid(t_data *data) {
    subdiv_grid(data->fract->p0.x, data->fract->p1.x, step_size, grid.x);
    subdiv_grid(data->fract->p0.y, data->fract->p1.y, step_size, grid.y);
    subdiv_grid(data->fract->p0.z, data->fract->p1.z, step_size, grid.z);
}
```

### Voxel Corner Definition
Each cube has 8 corners defined by offset vectors:
```c
void define_voxel(t_fract *fract, float s) {
    const float zz[2] = {-s/2, s/2};
    const float xx[4] = {-s/2, s/2, s/2, -s/2};
    const float yy[4] = {s/2, s/2, -s/2, -s/2};
    
    // Generate 8 corner offsets (2 z-levels × 4 corners each)
    for (unsigned i = 0; i < 2; i++) {
        for (unsigned j = 0; j < 4; j++) {
            fract->voxel[n].dx = xx[j];
            fract->voxel[n].dy = yy[j]; 
            fract->voxel[n].dz = zz[i];
            n++;
        }
    }
}
```

## Matrix and Poem Processing

### Alternative Parameter Input Methods

The system supports generating fractal parameters from:

1. **Matrix Files** (`-m` flag): Binary matrices converted to SHA256 hash
2. **Poem Files** (`-p` flag): Text content processed as character matrices
3. **Direct Input**: Manual specification of quaternion parameters

### Hash-Based Parameter Generation
```c
void matrix_hash(int **matrix, t_mat_conv_data *data) {
    char *mat_string = matrix_to_string(matrix);
    unsigned char *hash = SHA256(mat_string, strlen(mat_string), 0);
    get_coords_from_hash(hash, data);  // Extract quaternion from hash
}
```

## Performance Characteristics

### Computational Complexity
- **Grid Size**: O(n³) where n = grid_length/step_size
- **Julia Set Sampling**: O(n³ × max_iter) for volume sampling
- **Marching Cubes**: O(n³) for surface extraction
- **Triangle Count**: Varies significantly based on fractal complexity

### Memory Usage
- **Vertex Arrays**: ~8n³ vertices for computation
- **Triangle Storage**: Dynamic based on surface complexity
- **OpenGL Buffers**: Linear in final triangle count

### Critical Performance Factors
1. **Step Size**: Smaller values exponentially increase computation
2. **Iteration Count**: Higher values improve quality but slow computation
3. **Grid Bounds**: Larger volumes increase memory and computation
4. **Quaternion Parameters**: Affect fractal complexity and triangle density
