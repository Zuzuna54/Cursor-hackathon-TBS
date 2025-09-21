# Graphics Pipeline

## OpenGL Rendering Architecture

### Pipeline Overview
```
Triangle Data → Vertex Buffers → Shaders → Rasterization → Display
     ↓              ↓              ↓           ↓            ↓
  gl_retrieve    createVBO    makeShader   gl_render   glfwSwap
    _tris()        ()        Program()       ()       Buffers()
```

## OpenGL Context Setup

### Window and Context Initialization
```c
void init_gl(t_gl *gl) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    gl->window = glfwCreateWindow(800, 600, "Morphosis", NULL, NULL);
    glfwMakeContextCurrent(gl->window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
}
```

### Key Configuration
- **OpenGL Version**: 3.2 Core Profile
- **Window Size**: 800×600 pixels
- **Depth Testing**: Enabled for 3D rendering
- **Polygon Mode**: Wireframe (`GL_LINE`) for fractal visualization

## Vertex Buffer Management

### Data Conversion Process
1. **Triangle Array → Flat Array**: Convert `float3**` triangles to linear `float*` array
2. **Vertex Buffer Creation**: Upload data to GPU via `glBufferData`
3. **Vertex Array Object**: Configure vertex attribute pointers

```c
void gl_retrieve_tris(t_data *data) {
    data->gl->tris = malloc(data->gl->num_pts * sizeof(float));
    
    uint j = 0;
    for (uint i = 0; i < data->gl->num_tris; i++) {
        for (int c = 0; c < 3; c++) {  // 3 vertices per triangle
            data->gl->tris[j++] = data->triangles[i][c].x;
            data->gl->tris[j++] = data->triangles[i][c].y; 
            data->gl->tris[j++] = data->triangles[i][c].z;
        }
    }
}
```

### Buffer Configuration
```c
void createVBO(t_gl *gl, GLsizeiptr size, GLfloat *points) {
    glGenBuffers(1, &gl->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_DYNAMIC_DRAW);
}

void createVAO(t_gl *gl) {
    glGenVertexArrays(1, &gl->vao);
    glBindVertexArray(gl->vao);
}
```

## Shader System

### Vertex Shader (`vertex.shader`)
```glsl
#version 330 core

in vec3 pos;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0f);
}
```

### Fragment Shader (`fragment.shader`)
```glsl
#version 330 core

out vec4 color;

void main() {
    color = vec4(0.878f, 0.761f, 0.176f, 1.0f);  // Golden color
}
```

### Shader Compilation Pipeline
```c
GLuint createShader(GLenum type, char **src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar * const *)src, NULL);
    glCompileShader(shader);
    
    // Error checking
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        printf("Shader compilation failed: %s\n", buffer);
        exit(1);
    }
    return shader;
}
```

## Transformation Pipeline

### Matrix System
The application uses a complete 3D transformation pipeline:

1. **Model Matrix**: Object-to-world transformation
2. **View Matrix**: World-to-camera transformation  
3. **Projection Matrix**: Camera-to-screen transformation

### Camera Configuration
```c
t_matrix *initGlMatrices() {
    matrix->eye[0] = 1.5f;    // Camera position
    matrix->eye[1] = 1.5f;
    matrix->eye[2] = 0.0f;
    
    matrix->center[0] = 0.0f; // Look-at target
    matrix->center[1] = 0.0f;
    matrix->center[2] = 0.0f;
    
    matrix->up[0] = 0.8f;     // Up vector
    matrix->up[1] = 0.3f;
    matrix->up[2] = 0.7f;
}
```

### Real-time Animation
```c
void gl_render(t_gl *gl) {
    while (!glfwWindowShouldClose(gl->window)) {
        float time = glfwGetTime();
        float delta = time - old_time;
        
        // Rotate model and view matrices
        glm_mat4_identity(gl->matrix->model_mat);
        glm_rotate(gl->matrix->model_mat, 
                   0.25f * delta * glm_rad(180.0f), 
                   gl->matrix->up);
        
        glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);
        glfwSwapBuffers(gl->window);
    }
}
```

## Coordinate System and Scaling

### Coordinate Transformation
The fractal is generated in a standardized coordinate space and then scaled for rendering:

```c
void gl_scale_tris(t_gl *gl, float3 max, float3 min) {
    float delta_x = max.x - min.x;
    float delta_y = max.y - min.y;
    float delta_z = max.z - min.z;
    
    for (uint i = 0; i < gl->num_pts; i += 3) {
        gl->tris[i]   = ((gl->tris[i] - min.x) / delta_x) * 1.5f - 0.75f;
        gl->tris[i+1] = ((gl->tris[i+1] - min.y) / delta_y) * 1.5f - 0.75f;
        gl->tris[i+2] = ((gl->tris[i+2] - min.z) / delta_z) * 1.5f - 0.75f;
    }
}
```

## Complex Number Library

### 2D Complex Numbers
The system includes a complete complex number library with operations:

- **Basic**: Addition, multiplication, division
- **Advanced**: Power, square root, exponential, logarithm
- **Geometric**: Magnitude, argument (phase angle)

### Type System
```c
#if defined(DOUBLE_SUPPORT_AVAILABLE)
    typedef double2   cl_complex;
    typedef cl_double4 cl_quat;
    typedef double    TYPE;
#else
    typedef float2    cl_complex;
    typedef float4    cl_quat;
    typedef float     TYPE;
#endif
```

## Numerical Precision and Stability

### Iteration Parameters
- **Default Max Iterations**: 6 (relatively low for performance)
- **Escape Radius**: 2.0 (standard for Julia sets)
- **Default Step Size**: 0.05 (balance between quality and performance)

### Precision Considerations
- Uses single-precision floats by default
- Double precision available if OpenCL supports it
- Critical for maintaining mathematical accuracy in iterative computations

### Edge Cases Handled
- **Division by Zero**: Checked in interpolation functions
- **Infinite Values**: Escape condition prevents divergence
- **Boundary Conditions**: Proper handling of grid edges
