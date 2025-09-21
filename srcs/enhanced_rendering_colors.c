#include "morphosis.h"

/**
 * @brief Calculate face normal from three vertices
 * 
 * Computes the normal vector for a triangle using cross product of edge vectors.
 * The normal points outward from the surface.
 */
float3						calculate_face_normal(float3 v1, float3 v2, float3 v3)
{
	float3 edge1, edge2, normal;
	
	// Calculate edge vectors
	edge1.x = v2.x - v1.x;
	edge1.y = v2.y - v1.y;
	edge1.z = v2.z - v1.z;
	
	edge2.x = v3.x - v1.x;
	edge2.y = v3.y - v1.y;
	edge2.z = v3.z - v1.z;
	
	// Cross product: edge1 × edge2
	normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
	normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
	normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
	
	// Normalize the normal vector
	float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	if (length > 0.0001f) // Avoid division by zero
	{
		normal.x /= length;
		normal.y /= length;
		normal.z /= length;
	}
	else
	{
		// Default normal if calculation fails
		normal.x = 0.0f;
		normal.y = 1.0f;
		normal.z = 0.0f;
	}
	
	return normal;
}

/**
 * @brief Calculate normals for all triangles
 * 
 * Generates smooth vertex normals by averaging face normals of adjacent triangles.
 * This creates better lighting than flat shading.
 */
void						calculate_triangle_normals(float3 *triangles, int num_triangles, t_vertex_enhanced *enhanced_vertices)
{
	int i;
	
	printf("\x1b[36m[%s]\x1b[0m Calculating normals for %d triangles\n", __FILE__, num_triangles);
	
	// Calculate normals for each triangle (3 vertices per triangle)
	for (i = 0; i < num_triangles; i++)
	{
		int base_idx = i * 3;
		
		// Get the three vertices of this triangle
		float3 v1 = triangles[base_idx];
		float3 v2 = triangles[base_idx + 1];
		float3 v3 = triangles[base_idx + 2];
		
		// Calculate face normal
		float3 face_normal = calculate_face_normal(v1, v2, v3);
		
		// Assign the same normal to all three vertices of this triangle
		// (This gives flat shading - for smooth shading we'd need vertex sharing)
		enhanced_vertices[base_idx].normal = face_normal;
		enhanced_vertices[base_idx + 1].normal = face_normal;
		enhanced_vertices[base_idx + 2].normal = face_normal;
		
		// Copy positions
		enhanced_vertices[base_idx].position = v1;
		enhanced_vertices[base_idx + 1].position = v2;
		enhanced_vertices[base_idx + 2].position = v3;
	}
	
	printf("\x1b[32m[%s]\x1b[0m Normal calculation complete\n", __FILE__);
}

/**
 * @brief Generate colors for triangles based on various properties
 * 
 * Creates visually appealing color schemes based on:
 * - Surface orientation (normal direction)
 * - Position in space
 * - Fractal properties
 * - Distance from origin
 */
void						generate_triangle_colors(float3 *triangles, int num_triangles, t_vertex_enhanced *enhanced_vertices, t_data *data)
{
	int i;
	float3 center = {0.0f, 0.0f, 0.0f}; // Fractal center
	
	printf(\"\\x1b[36m[%s]\\x1b[0m Generating colors for %d triangles\\n\", __FILE__, num_triangles);
	
	for (i = 0; i < num_triangles; i++)
	{
		int base_idx = i * 3;
		
		// Calculate triangle center for color computation
		float3 tri_center;
		tri_center.x = (triangles[base_idx].x + triangles[base_idx + 1].x + triangles[base_idx + 2].x) / 3.0f;
		tri_center.y = (triangles[base_idx].y + triangles[base_idx + 1].y + triangles[base_idx + 2].y) / 3.0f;
		tri_center.z = (triangles[base_idx].z + triangles[base_idx + 1].z + triangles[base_idx + 2].z) / 3.0f;
		
		// Calculate distance from origin for color intensity
		float distance = sqrtf(tri_center.x * tri_center.x + tri_center.y * tri_center.y + tri_center.z * tri_center.z);
		
		// Get surface normal for orientation-based coloring
		float3 normal = enhanced_vertices[base_idx].normal;
		
		// Create color based on multiple factors
		float3 color;
		
		// Base color scheme: orientation-based coloring
		color.x = fabsf(normal.x) * 0.8f + 0.2f; // Red component based on X normal
		color.y = fabsf(normal.y) * 0.8f + 0.2f; // Green component based on Y normal  
		color.z = fabsf(normal.z) * 0.8f + 0.2f; // Blue component based on Z normal
		
		// Modulate by distance for depth perception
		float distance_factor = 1.0f - fminf(distance / 3.0f, 0.8f); // Closer = brighter
		color.x *= distance_factor;
		color.y *= distance_factor;
		color.z *= distance_factor;
		
		// Add position-based color variation for visual interest
		float pos_factor = 0.3f * (sinf(tri_center.x * 2.0f) + cosf(tri_center.y * 2.0f) + sinf(tri_center.z * 1.5f));
		color.x += pos_factor * 0.2f;
		color.y += pos_factor * 0.15f;
		color.z += pos_factor * 0.25f;
		
		// Add fractal parameter influence
		float param_influence = (data->fract->julia->c.x + data->fract->julia->c.y) * 0.1f;
		color.x += param_influence;
		color.y += param_influence * 0.5f;
		color.z -= param_influence * 0.3f;
		
		// Ensure colors stay in valid range [0.1, 1.0]
		color.x = fmaxf(0.1f, fminf(1.0f, color.x));
		color.y = fmaxf(0.1f, fminf(1.0f, color.y));
		color.z = fmaxf(0.1f, fminf(1.0f, color.z));
		
		// Apply the same color to all three vertices of this triangle
		enhanced_vertices[base_idx].color = color;
		enhanced_vertices[base_idx + 1].color = color;
		enhanced_vertices[base_idx + 2].color = color;
	}
	
	printf(\"\\x1b[32m[%s]\\x1b[0m Color generation complete\\n\", __FILE__);
}

/**
 * @brief Compile enhanced shaders with lighting support
 * 
 * Creates shader program with vertex and fragment shaders that support
 * position, normal, and color attributes with Phong lighting.
 */
GLuint						compile_enhanced_shaders(void)
{
	GLuint vertex_shader, fragment_shader, program;
	GLint success;
	char info_log[512];
	
	printf(\"\\x1b[36m[%s]\\x1b[0m Compiling enhanced shaders...\\n\", __FILE__);
	
	// Read and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	FILE *vs_file = fopen(\"shaders/vertex_enhanced.shader\", \"r\");
	if (!vs_file)
	{
		printf(\"\\x1b[31m[%s]\\x1b[0m Failed to open vertex_enhanced.shader\\n\", __FILE__);
		return 0;
	}
	
	// Read vertex shader source
	fseek(vs_file, 0, SEEK_END);
	long vs_length = ftell(vs_file);
	fseek(vs_file, 0, SEEK_SET);
	char *vs_source = malloc(vs_length + 1);
	fread(vs_source, 1, vs_length, vs_file);
	vs_source[vs_length] = '\\0';
	fclose(vs_file);
	
	glShaderSource(vertex_shader, 1, (const char**)&vs_source, NULL);
	glCompileShader(vertex_shader);
	
	// Check vertex shader compilation
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf(\"\\x1b[31m[%s]\\x1b[0m Vertex shader compilation failed: %s\\n\", __FILE__, info_log);
		free(vs_source);
		return 0;
	}
	
	// Read and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	FILE *fs_file = fopen(\"shaders/fragment_enhanced.shader\", \"r\");
	if (!fs_file)
	{
		printf(\"\\x1b[31m[%s]\\x1b[0m Failed to open fragment_enhanced.shader\\n\", __FILE__);
		free(vs_source);
		return 0;
	}
	
	// Read fragment shader source
	fseek(fs_file, 0, SEEK_END);
	long fs_length = ftell(fs_file);
	fseek(fs_file, 0, SEEK_SET);
	char *fs_source = malloc(fs_length + 1);
	fread(fs_source, 1, fs_length, fs_file);
	fs_source[fs_length] = '\\0';
	fclose(fs_file);
	
	glShaderSource(fragment_shader, 1, (const char**)&fs_source, NULL);
	glCompileShader(fragment_shader);
	
	// Check fragment shader compilation
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf(\"\\x1b[31m[%s]\\x1b[0m Fragment shader compilation failed: %s\\n\", __FILE__, info_log);
		free(vs_source);
		free(fs_source);
		return 0;
	}
	
	// Create and link shader program
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	
	// Check program linking
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, info_log);
		printf(\"\\x1b[31m[%s]\\x1b[0m Shader program linking failed: %s\\n\", __FILE__, info_log);
		free(vs_source);
		free(fs_source);
		return 0;
	}
	
	// Clean up
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	free(vs_source);
	free(fs_source);
	
	printf(\"\\x1b[32m[%s]\\x1b[0m Enhanced shaders compiled successfully\\n\", __FILE__);
	return program;
}

/**
 * @brief Initialize enhanced rendering system
 * 
 * Sets up enhanced vertex buffers, shaders, and uniform locations
 * for advanced rendering with lighting and colors.
 */
void						init_enhanced_rendering(t_gl *gl)
{
	printf(\"\\x1b[36m[%s]\\x1b[0m Initializing enhanced rendering system...\\n\", __FILE__);
	
	// Compile enhanced shaders
	gl->enhanced_program = compile_enhanced_shaders();
	if (gl->enhanced_program == 0)
	{
		printf(\"\\x1b[31m[%s]\\x1b[0m Failed to compile enhanced shaders\\n\", __FILE__);
		gl->use_enhanced_rendering = 0;
		return;
	}
	
	// Get uniform locations
	glUseProgram(gl->enhanced_program);
	gl->enhanced_uniforms[0] = glGetUniformLocation(gl->enhanced_program, \"model\");
	gl->enhanced_uniforms[1] = glGetUniformLocation(gl->enhanced_program, \"view\");
	gl->enhanced_uniforms[2] = glGetUniformLocation(gl->enhanced_program, \"proj\");
	gl->enhanced_uniforms[3] = glGetUniformLocation(gl->enhanced_program, \"normalMatrix\");
	gl->enhanced_uniforms[4] = glGetUniformLocation(gl->enhanced_program, \"lightPos\");
	gl->enhanced_uniforms[5] = glGetUniformLocation(gl->enhanced_program, \"lightColor\");
	gl->enhanced_uniforms[6] = glGetUniformLocation(gl->enhanced_program, \"viewPos\");
	gl->enhanced_uniforms[7] = glGetUniformLocation(gl->enhanced_program, \"renderMode\");
	
	// Generate enhanced VBO
	glGenBuffers(1, &gl->enhanced_vbo);
	
	// Initialize enhanced vertices buffer (will be allocated when needed)
	gl->enhanced_vertices = NULL;
	gl->use_enhanced_rendering = 1;
	
	printf(\"\\x1b[32m[%s]\\x1b[0m Enhanced rendering system initialized\\n\", __FILE__);
}

/**
 * @brief Update enhanced vertices with current triangle data
 * 
 * Regenerates normals and colors for all triangles when fractal parameters change.
 */
void						update_enhanced_vertices(t_data *data)
{
	t_gl *gl = data->gl;
	
	if (!gl->use_enhanced_rendering || !gl->tris || gl->num_tris <= 0)
		return;
	
	printf(\"\\x1b[36m[%s]\\x1b[0m Updating enhanced vertices for %d triangles\\n\", __FILE__, gl->num_tris);
	
	// Allocate enhanced vertices buffer
	int num_vertices = gl->num_tris * 3; // 3 vertices per triangle
	if (gl->enhanced_vertices)
		free(gl->enhanced_vertices);
	
	gl->enhanced_vertices = malloc(sizeof(t_vertex_enhanced) * num_vertices);
	if (!gl->enhanced_vertices)
	{
		printf(\"\\x1b[31m[%s]\\x1b[0m Failed to allocate enhanced vertices buffer\\n\", __FILE__);
		return;
	}
	
	// Calculate normals and positions
	calculate_triangle_normals(gl->tris, gl->num_tris, gl->enhanced_vertices);
	
	// Generate colors
	generate_triangle_colors(gl->tris, gl->num_tris, gl->enhanced_vertices, data);
	
	// Update VBO with enhanced vertex data
	glBindBuffer(GL_ARRAY_BUFFER, gl->enhanced_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertex_enhanced) * num_vertices, 
				 gl->enhanced_vertices, GL_STATIC_DRAW);
	
	printf(\"\\x1b[32m[%s]\\x1b[0m Enhanced vertices updated\\n\", __FILE__);
}

/**
 * @brief Render using enhanced shaders with lighting and colors
 * 
 * Replaces the standard rendering pipeline when in colored mode
 * to provide proper lighting and color-coded surfaces.
 */
void						render_enhanced(t_data *data)
{
	t_gl *gl = data->gl;
	
	if (!gl->use_enhanced_rendering || !gl->enhanced_vertices || gl->num_tris <= 0)
		return;
	
	// Use enhanced shader program
	glUseProgram(gl->enhanced_program);
	
	// Bind enhanced VBO and set up vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, gl->enhanced_vbo);
	
	// Position attribute (location 0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vertex_enhanced), (void*)0);
	
	// Normal attribute (location 1)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(t_vertex_enhanced), (void*)(sizeof(float3)));
	
	// Color attribute (location 2)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(t_vertex_enhanced), (void*)(2 * sizeof(float3)));
	
	// Set uniform matrices
	glUniformMatrix4fv(gl->enhanced_uniforms[0], 1, GL_FALSE, (float *)gl->matrix->model_mat);
	glUniformMatrix4fv(gl->enhanced_uniforms[1], 1, GL_FALSE, (float *)gl->matrix->view_mat);
	glUniformMatrix4fv(gl->enhanced_uniforms[2], 1, GL_FALSE, (float *)gl->matrix->proj_mat);
	
	// Calculate and set normal matrix (for proper normal transformation)
	mat3 normal_matrix;
	mat4 model_view;
	glm_mat4_mul(gl->matrix->view_mat, gl->matrix->model_mat, model_view);
	glm_mat4_pick3t(model_view, normal_matrix);
	glm_mat3_inv(normal_matrix, normal_matrix);
	glm_mat3_transpose(normal_matrix);
	glUniformMatrix3fv(gl->enhanced_uniforms[3], 1, GL_FALSE, (float *)normal_matrix);
	
	// Set lighting parameters
	float light_pos[3] = {2.0f, 4.0f, 2.0f}; // Light position
	float light_color[3] = {1.0f, 1.0f, 1.0f}; // White light
	glUniform3fv(gl->enhanced_uniforms[4], 1, light_pos);
	glUniform3fv(gl->enhanced_uniforms[5], 1, light_color);
	glUniform3fv(gl->enhanced_uniforms[6], 1, gl->matrix->eye); // Camera position
	glUniform1i(gl->enhanced_uniforms[7], gl->render_mode); // Render mode
	
	// Render triangles
	glDrawArrays(GL_TRIANGLES, 0, gl->num_tris * 3);
	
	// Clean up
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

/**
 * @brief Clean up enhanced rendering resources
 * 
 * Frees allocated memory and OpenGL resources for enhanced rendering.
 */
void						cleanup_enhanced_rendering(t_gl *gl)
{
	if (gl->enhanced_vertices)
	{
		free(gl->enhanced_vertices);
		gl->enhanced_vertices = NULL;
	}
	
	if (gl->enhanced_vbo)
	{
		glDeleteBuffers(1, &gl->enhanced_vbo);
		gl->enhanced_vbo = 0;
	}
	
	if (gl->enhanced_program)
	{
		glDeleteProgram(gl->enhanced_program);
		gl->enhanced_program = 0;
	}
	
	gl->use_enhanced_rendering = 0;
	
	printf(\"\\x1b[32m[%s]\\x1b[0m Enhanced rendering cleanup complete\\n\", __FILE__);
}
