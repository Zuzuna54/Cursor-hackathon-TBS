#version 330 core

// Input vertex attributes
in vec3                 pos;        // Vertex position
in vec3                 normal;     // Vertex normal (will be calculated)

// Uniform matrices
uniform mat4            model;      // Model transformation matrix
uniform mat4            view;       // View transformation matrix  
uniform mat4            proj;       // Projection transformation matrix
uniform mat3            normalMat;  // Normal transformation matrix

// Output to fragment shader
out vec3                fragPos;    // World space position
out vec3                fragNormal; // World space normal
out float               fragDepth;  // Normalized depth for coloring

void                    main()
{
    // Transform vertex position
    vec4 worldPos = model * vec4(pos, 1.0f);
    fragPos = worldPos.xyz;
    
    // Transform normal to world space
    fragNormal = normalize(normalMat * normal);
    
    // Calculate normalized depth (0.0 = near, 1.0 = far)
    vec4 viewPos = view * worldPos;
    fragDepth = (-viewPos.z - 1.0) / 20.0; // Adjust range as needed
    fragDepth = clamp(fragDepth, 0.0, 1.0);
    
    // Final position transformation
    gl_Position = proj * view * model * vec4(pos, 1.0f);
}
