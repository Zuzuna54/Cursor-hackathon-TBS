#version 330 core

// Input from vertex shader
in vec3 FragPos;     // Fragment position in world space
in vec3 Normal;      // Fragment normal in world space  
in vec3 VertexColor; // Vertex color
in vec3 LightPos;    // Light position
in vec3 LightColor;  // Light color
in vec3 ViewPos;     // View position
flat in int RenderMode; // Render mode

// Output color
out vec4 FragColor;

void main()
{
    if (RenderMode == 0) // Wireframe mode
    {
        // Simple wireframe color
        FragColor = vec4(0.8, 0.8, 0.8, 1.0); // Light gray
    }
    else if (RenderMode == 1) // Solid mode
    {
        // Simple solid color with basic lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(LightPos - FragPos);
        
        // Basic diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * LightColor;
        
        // Ambient lighting
        vec3 ambient = 0.3 * LightColor;
        
        // Combine lighting with base color
        vec3 baseColor = vec3(0.878, 0.761, 0.176); // Original gold color
        vec3 result = (ambient + diffuse) * baseColor;
        
        FragColor = vec4(result, 1.0);
    }
    else // Colored mode (RenderMode == 2)
    {
        // Enhanced colored mode with full Phong lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(LightPos - FragPos);
        vec3 viewDir = normalize(ViewPos - FragPos);
        
        // Ambient lighting
        vec3 ambient = 0.2 * LightColor;
        
        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * LightColor;
        
        // Specular lighting (Phong)
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = 0.5 * spec * LightColor;
        
        // Combine lighting with vertex color
        vec3 result = (ambient + diffuse + specular) * VertexColor;
        
        // Add some rim lighting for better visibility
        float rimFactor = 1.0 - max(dot(viewDir, norm), 0.0);
        vec3 rimLight = 0.3 * pow(rimFactor, 2.0) * vec3(0.5, 0.8, 1.0); // Blue rim
        
        result += rimLight;
        
        FragColor = vec4(result, 1.0);
    }
}
