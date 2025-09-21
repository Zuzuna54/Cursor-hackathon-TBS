#version 330 core

// Input attributes
in vec3 pos;       // Vertex position
in vec3 normal;    // Surface normal
in vec3 color;     // Vertex color

// Uniform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix; // Normal transformation matrix

// Uniforms for lighting
uniform vec3 lightPos;     // Light position in world space
uniform vec3 lightColor;   // Light color
uniform vec3 viewPos;      // Camera position in world space
uniform int renderMode;    // 0=wireframe, 1=solid, 2=colored

// Output to fragment shader
out vec3 FragPos;     // Fragment position in world space
out vec3 Normal;      // Fragment normal in world space
out vec3 VertexColor; // Vertex color
out vec3 LightPos;    // Light position
out vec3 LightColor;  // Light color
out vec3 ViewPos;     // View position
flat out int RenderMode; // Render mode (flat = no interpolation)

void main()
{
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(pos, 1.0));
    
    // Transform normal to world space (handle non-uniform scaling)
    Normal = normalMatrix * normal;
    
    // Pass through vertex color and lighting parameters
    VertexColor = color;
    LightPos = lightPos;
    LightColor = lightColor;
    ViewPos = viewPos;
    RenderMode = renderMode;
    
    // Final position transformation
    gl_Position = proj * view * vec4(FragPos, 1.0);
}
