#version 330 core

in vec3                 pos;

uniform mat4            model;
uniform mat4            view;
uniform mat4            proj;

// Output to fragment shader
out vec3                worldPos;   // World position for coloring

void                    main()
{
    // Transform to world space for coloring
    vec4 worldPosition = model * vec4(pos, 1.0f);
    worldPos = worldPosition.xyz;
    
    gl_Position = proj * view * worldPosition;
}