#version 330 core

// Input from vertex shader
in vec3                 worldPos;   // World position for coloring

// Uniforms
uniform int             renderMode; // 0=wireframe, 1=solid, 2=colored
uniform mat4            model;      // Model matrix for lighting calculations
uniform mat4            view;       // View matrix for lighting calculations

out vec4                color;

// Enhanced color palette based on world position
vec3 getEnhancedPositionColor(vec3 pos)
{
    // Create rich color variation using multiple harmonics
    float r = sin(pos.x * 2.0) * 0.3 + sin(pos.x * 8.0) * 0.2 + 0.5;
    float g = sin(pos.y * 2.5 + 1.57) * 0.3 + sin(pos.y * 6.0) * 0.2 + 0.5;
    float b = sin(pos.z * 3.0 + 3.14) * 0.3 + sin(pos.z * 4.0) * 0.2 + 0.5;
    
    // Clamp to valid range
    r = clamp(r, 0.1, 0.9);
    g = clamp(g, 0.1, 0.9);
    b = clamp(b, 0.1, 0.9);
    
    return vec3(r, g, b);
}

// Generate depth-based color gradient
vec3 getDepthColor(vec3 pos)
{
    float depth = (pos.z + 2.0) * 0.25; // Normalize depth
    depth = clamp(depth, 0.0, 1.0);
    
    // Create a rainbow-like gradient
    vec3 colors[6];
    colors[0] = vec3(0.8, 0.2, 0.2);  // Deep red
    colors[1] = vec3(0.9, 0.5, 0.1);  // Orange
    colors[2] = vec3(0.9, 0.9, 0.2);  // Yellow
    colors[3] = vec3(0.2, 0.8, 0.3);  // Green
    colors[4] = vec3(0.2, 0.4, 0.9);  // Blue
    colors[5] = vec3(0.6, 0.2, 0.8);  // Purple
    
    float scaled = depth * 5.0;
    int index = int(floor(scaled));
    float frac = fract(scaled);
    
    if (index >= 5) return colors[5];
    if (index < 0) return colors[0];
    
    return mix(colors[index], colors[index + 1], frac);
}

// Approximate surface normal using screen-space derivatives
vec3 calculateApproximateNormal(vec3 pos)
{
    // Use built-in derivatives to approximate surface normal
    vec3 dFdxPos = dFdx(pos);
    vec3 dFdyPos = dFdy(pos);
    
    // Cross product gives us the surface normal
    vec3 normal = normalize(cross(dFdxPos, dFdyPos));
    
    // Ensure normal faces toward camera (flip if needed)
    vec3 viewDir = normalize(-pos); // Assuming pos is in view space
    if (dot(normal, viewDir) < 0.0) {
        normal = -normal;
    }
    
    return normal;
}

// Apply lighting to enhance surface visibility
vec3 applyLighting(vec3 baseColor, vec3 pos, vec3 normal)
{
    // Light positions (multiple lights for better illumination)
    vec3 light1 = vec3(2.0, 2.0, 2.0);
    vec3 light2 = vec3(-1.5, 1.0, 1.5);
    vec3 light3 = vec3(0.0, -1.0, 2.0);
    
    // Ambient lighting
    vec3 ambient = baseColor * 0.3;
    
    // Diffuse lighting from multiple sources
    vec3 lightDir1 = normalize(light1 - pos);
    vec3 lightDir2 = normalize(light2 - pos);
    vec3 lightDir3 = normalize(light3 - pos);
    
    float diff1 = max(dot(normal, lightDir1), 0.0);
    float diff2 = max(dot(normal, lightDir2), 0.0);
    float diff3 = max(dot(normal, lightDir3), 0.0);
    
    vec3 diffuse = baseColor * (diff1 * 0.5 + diff2 * 0.3 + diff3 * 0.2);
    
    // Simple specular highlight
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir1 = reflect(-lightDir1, normal);
    float spec = pow(max(dot(viewDir, reflectDir1), 0.0), 32.0);
    vec3 specular = vec3(0.3, 0.3, 0.3) * spec;
    
    return ambient + diffuse + specular;
}

void                    main()
{
    if (renderMode == 2) // Enhanced colored mode
    {
        // Get base colors
        vec3 posColor = getEnhancedPositionColor(worldPos);
        vec3 depthColor = getDepthColor(worldPos);
        
        // Blend position and depth colors
        vec3 baseColor = mix(posColor, depthColor, 0.4);
        
        // Calculate approximate surface normal
        vec3 normal = calculateApproximateNormal(worldPos);
        
        // Apply lighting for surface definition
        vec3 litColor = applyLighting(baseColor, worldPos, normal);
        
        // Add subtle edge enhancement
        float edgeFactor = 1.0 - abs(dot(normal, normalize(-worldPos)));
        edgeFactor = pow(edgeFactor, 2.0);
        vec3 edgeColor = vec3(0.9, 0.9, 0.9) * edgeFactor * 0.2;
        
        color = vec4(litColor + edgeColor, 1.0);
    }
    else // Wireframe and solid modes
    {
        color = vec4(0.878f, 0.761f, 0.176f, 1.0f);
    }
}