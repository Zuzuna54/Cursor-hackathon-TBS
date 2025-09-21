#include "morphosis.h"

/**
 * @brief Initialize enhanced shaders for colored rendering mode
 * 
 * Loads and compiles the enhanced vertex and fragment shaders that support
 * normals, lighting, and advanced coloring techniques.
 */
void						init_enhanced_shaders(t_gl *gl)
{
	printf("\x1b[36m[%s]\x1b[0m Initializing enhanced shaders for colored mode\n", __FILE__);
	
	// Load and compile enhanced vertex shader
	gl->enhanced_vertex_shader = compileShader(VERTEX_ENHANCED_SHADER_PATH, GL_VERTEX_SHADER);
	if (gl->enhanced_vertex_shader == 0) {
		printf("\x1b[31m[%s]\x1b[0m Failed to compile enhanced vertex shader\n", __FILE__);
		return;
	}
	
	// Load and compile enhanced fragment shader
	gl->enhanced_fragment_shader = compileShader(FRAGMENT_ENHANCED_SHADER_PATH, GL_FRAGMENT_SHADER);
	if (gl->enhanced_fragment_shader == 0) {
		printf("\x1b[31m[%s]\x1b[0m Failed to compile enhanced fragment shader\n", __FILE__);
		return;
	}
	
	// Create enhanced shader program
	gl->enhanced_shader_program = createEnhancedProgram(gl->enhanced_vertex_shader, gl->enhanced_fragment_shader);
	if (gl->enhanced_shader_program == 0) {
		printf("\x1b[31m[%s]\x1b[0m Failed to create enhanced shader program\n", __FILE__);
		return;
	}
	
	printf("\x1b[32m[%s]\x1b[0m Enhanced shaders initialized successfully\n", __FILE__);
}

/**
 * @brief Clean up enhanced shader resources
 */
void						cleanup_enhanced_shaders(t_gl *gl)
{
	if (gl->enhanced_shader_program != 0) {
		glDeleteProgram(gl->enhanced_shader_program);
		gl->enhanced_shader_program = 0;
	}
	
	if (gl->enhanced_vertex_shader != 0) {
		glDeleteShader(gl->enhanced_vertex_shader);
		gl->enhanced_vertex_shader = 0;
	}
	
	if (gl->enhanced_fragment_shader != 0) {
		glDeleteShader(gl->enhanced_fragment_shader);
		gl->enhanced_fragment_shader = 0;
	}
	
	if (gl->vertex_normals != NULL) {
		free(gl->vertex_normals);
		gl->vertex_normals = NULL;
	}
	
	if (gl->normal_buffer != 0) {
		glDeleteBuffers(1, &gl->normal_buffer);
		gl->normal_buffer = 0;
	}
}

/**
 * @brief Calculate vertex normals for lighting
 * 
 * Computes per-vertex normals by averaging face normals of adjacent triangles.
 * This enables proper lighting calculations in the enhanced shaders.
 */
void						calculate_vertex_normals(t_data *data)
{
	t_gl *gl = data->gl;
	uint num_vertices = gl->num_pts;
	
	printf("\x1b[36m[%s]\x1b[0m Calculating vertex normals for %d vertices\n", __FILE__, num_vertices);
	
	// Allocate memory for normals (3 floats per vertex)
	if (gl->vertex_normals != NULL) {
		free(gl->vertex_normals);
	}
	gl->vertex_normals = (float *)malloc(num_vertices * sizeof(float));
	if (!gl->vertex_normals) {
		printf("\x1b[31m[%s]\x1b[0m Failed to allocate memory for vertex normals\n", __FILE__);
		return;
	}
	
	// Initialize all normals to zero
	memset(gl->vertex_normals, 0, num_vertices * sizeof(float));
	
	// Calculate face normals and accumulate to vertex normals
	for (uint i = 0; i < gl->num_tris; i++) {
		// Get the three vertices of the triangle
		float3 v0 = data->triangles[i][0];
		float3 v1 = data->triangles[i][1];
		float3 v2 = data->triangles[i][2];
		
		// Calculate edge vectors
		float3 edge1 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
		float3 edge2 = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};
		
		// Calculate face normal using cross product
		float3 face_normal;
		face_normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
		face_normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
		face_normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
		
		// Normalize the face normal
		float length = sqrtf(face_normal.x * face_normal.x + 
							face_normal.y * face_normal.y + 
							face_normal.z * face_normal.z);
		if (length > 0.0001f) { // Avoid division by zero
			face_normal.x /= length;
			face_normal.y /= length;
			face_normal.z /= length;
		}
		
		// Add this face normal to each vertex of the triangle
		uint base_index = i * 9; // 3 vertices * 3 components each
		
		// Vertex 0
		gl->vertex_normals[base_index + 0] += face_normal.x;
		gl->vertex_normals[base_index + 1] += face_normal.y;
		gl->vertex_normals[base_index + 2] += face_normal.z;
		
		// Vertex 1
		gl->vertex_normals[base_index + 3] += face_normal.x;
		gl->vertex_normals[base_index + 4] += face_normal.y;
		gl->vertex_normals[base_index + 5] += face_normal.z;
		
		// Vertex 2
		gl->vertex_normals[base_index + 6] += face_normal.x;
		gl->vertex_normals[base_index + 7] += face_normal.y;
		gl->vertex_normals[base_index + 8] += face_normal.z;
	}
	
	// Normalize all accumulated vertex normals
	for (uint i = 0; i < num_vertices; i += 3) {
		float x = gl->vertex_normals[i];
		float y = gl->vertex_normals[i + 1];
		float z = gl->vertex_normals[i + 2];
		
		float length = sqrtf(x * x + y * y + z * z);
		if (length > 0.0001f) { // Avoid division by zero
			gl->vertex_normals[i] = x / length;
			gl->vertex_normals[i + 1] = y / length;
			gl->vertex_normals[i + 2] = z / length;
		} else {
			// Default normal pointing up if calculation failed
			gl->vertex_normals[i] = 0.0f;
			gl->vertex_normals[i + 1] = 1.0f;
			gl->vertex_normals[i + 2] = 0.0f;
		}
	}
	
	printf("\x1b[32m[%s]\x1b[0m Vertex normals calculated successfully\n", __FILE__);
}

/**
 * @brief Setup enhanced vertex attributes for colored rendering
 * 
 * Configures OpenGL vertex attributes to include both position and normal data
 * for the enhanced shaders.
 */
void						setup_enhanced_vertex_attributes(t_gl *gl)
{
	// Generate and bind normal buffer
	if (gl->normal_buffer == 0) {
		glGenBuffers(1, &gl->normal_buffer);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, gl->normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, gl->num_pts * sizeof(float), gl->vertex_normals, GL_STATIC_DRAW);
	
	// Setup normal attribute (location 1)
	GLuint normal_attrib = glGetAttribLocation(gl->enhanced_shader_program, "normal");
	glVertexAttribPointer(normal_attrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(normal_attrib);
	
	// Bind position buffer and setup position attribute (location 0)
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	GLuint pos_attrib = glGetAttribLocation(gl->enhanced_shader_program, "pos");
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(pos_attrib);
	
	printf("\x1b[32m[%s]\x1b[0m Enhanced vertex attributes configured\n", __FILE__);
}

/**
 * @brief Use enhanced shaders and setup uniforms for colored rendering
 * 
 * Activates the enhanced shader program and sets up all necessary uniforms
 * including matrices, lighting parameters, and render mode.
 */
void						use_enhanced_shaders(t_gl *gl, t_data *data)
{
	// Use the enhanced shader program
	glUseProgram(gl->enhanced_shader_program);
	
	// Setup matrix uniforms
	GLuint model_loc = glGetUniformLocation(gl->enhanced_shader_program, "model");
	GLuint view_loc = glGetUniformLocation(gl->enhanced_shader_program, "view");
	GLuint proj_loc = glGetUniformLocation(gl->enhanced_shader_program, "proj");
	GLuint normal_mat_loc = glGetUniformLocation(gl->enhanced_shader_program, "normalMat");
	
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)gl->matrix->model_mat);
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)gl->matrix->view_mat);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float *)gl->matrix->projection_mat);
	
	// Calculate and set normal matrix (inverse transpose of model matrix)
	mat3 normal_matrix;
	glm_mat4_pick3t(gl->matrix->model_mat, normal_matrix);
	glm_mat3_inv(normal_matrix, normal_matrix);
	glm_mat3_transpose(normal_matrix);
	glUniformMatrix3fv(normal_mat_loc, 1, GL_FALSE, (float *)normal_matrix);
	
	// Setup rendering mode uniform
	GLuint render_mode_loc = glGetUniformLocation(gl->enhanced_shader_program, "renderMode");
	glUniform1i(render_mode_loc, gl->render_mode);
	
	// Setup lighting uniforms
	GLuint light_pos_loc = glGetUniformLocation(gl->enhanced_shader_program, "lightPos");
	GLuint view_pos_loc = glGetUniformLocation(gl->enhanced_shader_program, "viewPos");
	
	// Set light position (above and to the right of the fractal)
	vec3 light_pos = {5.0f, 5.0f, 5.0f};
	glUniform3fv(light_pos_loc, 1, light_pos);
	
	// Set camera/view position
	glUniform3fv(view_pos_loc, 1, gl->matrix->eye);
	
	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("\x1b[31m[%s]\x1b[0m OpenGL error in enhanced shader setup: 0x%x\n", __FILE__, error);
	}
}
