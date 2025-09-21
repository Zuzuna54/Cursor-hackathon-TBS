#version 330 core

// Input from vertex shader
in vec3                 fragPos;    // World space position
in vec3                 fragNormal; // World space normal
in float                fragDepth;  // Normalized depth

// Uniforms for lighting and rendering mode
uniform int             renderMode; // 0=wireframe, 1=solid, 2=colored
uniform vec3            lightPos;   // Light position in world space
uniform vec3            viewPos;    // Camera position in world space

// Output color
out vec4                color;

// Color palette for depth-based coloring
vec3 getDepthColor(float depth)
{
    // Create a rainbow-like gradient based on depth
    vec3 colors[6];
    colors[0] = vec3(1.0, 0.0, 0.0);  // Red (closest)
    colors[1] = vec3(1.0, 0.5, 0.0);  // Orange
    colors[2] = vec3(1.0, 1.0, 0.0);  // Yellow
    colors[3] = vec3(0.0, 1.0, 0.0);  // Green
    colors[4] = vec3(0.0, 0.0, 1.0);  // Blue
    colors[5] = vec3(0.5, 0.0, 1.0);  // Purple (farthest)
    
    // Interpolate between colors
    float scaled = depth * 5.0; // Scale to 0-5 range
    int index = int(floor(scaled));
    float frac = fract(scaled);
    
    if (index >= 5) return colors[5];
    if (index < 0) return colors[0];
    
    return mix(colors[index], colors[index + 1], frac);
}

// Normal-based coloring for interesting visual effects
vec3 getNormalColor(vec3 normal)
{
    // Map normal direction to colors
    return (normal + 1.0) * 0.5; // Convert from [-1,1] to [0,1]
}

// Advanced fractal-inspired coloring
vec3 getFractalColor(vec3 pos, vec3 normal, float depth)
{
    // Create color based on position and normal
    float r = sin(pos.x * 2.0 + depth * 3.14159) * 0.5 + 0.5;
    float g = sin(pos.y * 2.0 + depth * 3.14159 * 1.3) * 0.5 + 0.5;
    float b = sin(pos.z * 2.0 + depth * 3.14159 * 0.7) * 0.5 + 0.5;
    
    // Modulate with normal for more interesting patterns
    vec3 baseColor = vec3(r, g, b);
    vec3 normalColor = getNormalColor(normal);
    
    // Blend base color with normal-based color
    return mix(baseColor, normalColor, 0.3);
}

void                    main()
{
    vec3 baseColor;
    
    // Choose coloring method based on render mode
    if (renderMode == 0) 
    {
        // Wireframe mode - simple white
        baseColor = vec3(0.9, 0.9, 0.9);
    }
    else if (renderMode == 1)
    {
        // Solid mode - original gold color
        baseColor = vec3(0.878, 0.761, 0.176);
    }
    else
    {
        // Colored mode - enhanced coloring
        // Combine different coloring techniques
        vec3 depthColor = getDepthColor(fragDepth);
        vec3 fractalColor = getFractalColor(fragPos, fragNormal, fragDepth);
        
        // Blend the colors for rich visual effect
        baseColor = mix(depthColor, fractalColor, 0.6);
    }
    
    // Calculate lighting for all modes (except wireframe)
    if (renderMode > 0)
    {
        // Normalize the normal vector
        vec3 norm = normalize(fragNormal);
        
        // Ambient lighting
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        
        // Diffuse lighting
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        
        // Specular lighting (subtle)
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = 0.2 * spec * vec3(1.0, 1.0, 1.0);
        
        // Combine lighting with base color
        vec3 result = (ambient + diffuse + specular) * baseColor;
        color = vec4(result, 1.0);
    }
    else
    {
        // Wireframe mode - no lighting
        color = vec4(baseColor, 1.0);
    }
}
