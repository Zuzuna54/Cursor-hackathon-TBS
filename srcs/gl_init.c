#include "morphosis.h"

t_matrix 					*initGlMatrices(void)
{
	t_matrix 				*matrix;

	if (!(matrix = (t_matrix *)malloc(sizeof(t_matrix))))
		error(MALLOC_FAIL_ERR, NULL);

	glm_mat4_identity(matrix->model_mat);
	glm_mat4_identity(matrix->projection_mat);
	glm_mat4_identity(matrix->view_mat);

	matrix->eye[0] = 1.5f;
	matrix->eye[1] = 1.5f;
	matrix->eye[2] = 0.0f;

	matrix->center[0] = 0.0f;
	matrix->center[1] = 0.0f;
	matrix->center[2] = 0.0f;

	matrix->up[0] = 0.8f;
	matrix->up[1] = 0.3f;
	matrix->up[2] = 0.7f;

	return matrix;
}

t_gl						*init_gl_struct(void)
{
	t_gl					*gl;

	if (!(gl = (t_gl *)malloc(sizeof(t_gl))))
		error(MALLOC_FAIL_ERR, NULL);
	gl->window = NULL;
	gl->export = 0;
	gl->shaderProgram = 0;
	gl->vertexShader = 0;
	gl->fragmentShader = 0;
	gl->vbo = 0;
	gl->vao = 0;
	gl->tris = NULL;
	gl->num_pts = 0;
	gl->matrix = initGlMatrices();
	
	// Initialize enhanced rendering features
	gl->render_mode = 0;			// Start with wireframe
	gl->needs_regeneration = 0;		// No regeneration needed initially
	gl->zoom_factor = 1.0f;			// Default zoom
	gl->auto_rotate = 1;			// Enable auto rotation by default
	
	// Initialize mouse camera controls
	gl->last_mouse_x = 0.0;
	gl->last_mouse_y = 0.0;
	gl->mouse_dragging = 0;
	gl->camera_rotation_x = 0.0f;
	gl->camera_rotation_y = 0.0f;
	
	// Initialize enhanced rendering
	gl->enhanced_vertices = NULL;
	gl->enhanced_vbo = 0;
	gl->enhanced_program = 0;
	gl->use_enhanced_rendering = 0;
	
	return gl;
}

