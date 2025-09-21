#include "morphosis.h"

/**
 * @brief Handle render mode changes (wireframe/solid/colored)
 * 
 * Updates OpenGL polygon mode and other rendering settings
 * based on the current render mode.
 */
void						handle_render_mode_change(t_gl *gl)
{
	switch (gl->render_mode)
	{
		case 0: // Wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.0f);
			glUseProgram(gl->shaderProgram); // Use basic shaders
			break;
			
		case 1: // Solid mode  
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glUseProgram(gl->shaderProgram); // Use basic shaders
			break;
			
		case 2: // Enhanced colored mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			// Enhanced colored mode will use enhanced shaders
			// Shader switching will be handled in the render loop
			break;
			
		default:
			gl->render_mode = 0; // Reset to wireframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glUseProgram(gl->shaderProgram);
			break;
	}
}

/**
 * @brief Handle camera controls (zoom and basic movement)
 * 
 * Implements zoom controls and prepares for mouse-based camera rotation.
 */
void						handle_camera_controls(GLFWwindow *window, t_gl *gl)
{
	static double last_camera_time = 0.0;
	double current_time = glfwGetTime();
	
	// Prevent camera spam (50ms delay for smooth movement)
	if (current_time - last_camera_time < 0.05)
		return;
	
	// Zoom controls
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		gl->zoom_factor *= 1.1f;
		if (gl->zoom_factor > 5.0f) gl->zoom_factor = 5.0f;
		update_camera_position(gl);
		printf("\x1b[36m[%s]\x1b[0m Zoom: %.2fx\n", __FILE__, gl->zoom_factor);
		last_camera_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		gl->zoom_factor /= 1.1f;
		if (gl->zoom_factor < 0.1f) gl->zoom_factor = 0.1f;
		update_camera_position(gl);
		printf("\x1b[36m[%s]\x1b[0m Zoom: %.2fx\n", __FILE__, gl->zoom_factor);
		last_camera_time = current_time;
	}
}

/**
 * @brief Update camera position based on zoom and rotation
 * 
 * Calculates camera position using spherical coordinates for smooth
 * rotation and zoom control.
 */
void						update_camera_position(t_gl *gl)
{
	float radius = 3.0f / gl->zoom_factor;
	float x_rot = gl->camera_rotation_x;
	float y_rot = gl->camera_rotation_y;
	
	// Spherical coordinates to Cartesian
	gl->matrix->eye[0] = radius * cos(x_rot) * cos(y_rot);
	gl->matrix->eye[1] = radius * sin(x_rot);
	gl->matrix->eye[2] = radius * cos(x_rot) * sin(y_rot);
	
	// Always look at origin
	gl->matrix->center[0] = 0.0f;
	gl->matrix->center[1] = 0.0f;
	gl->matrix->center[2] = 0.0f;
	
	// Update up vector for proper orientation
	gl->matrix->up[0] = 0.0f;
	gl->matrix->up[1] = 1.0f;
	gl->matrix->up[2] = 0.0f;
}

/**
 * @brief Print current parameter information to console
 * 
 * Displays all current fractal parameters, rendering settings,
 * and control instructions.
 */
void						print_parameter_info(t_data *data)
{
	printf("\x1b[32m[%s]\x1b[0m ========== MORPHOSIS PARAMETERS ==========\n", __FILE__);
	printf("\x1b[36m[%s]\x1b[0m Julia Set Parameters:\n", __FILE__);
	printf("  C = (%.3f, %.3f, %.3f, %.3f)\n", 
		   data->fract->julia->c.x, data->fract->julia->c.y, 
		   data->fract->julia->c.z, data->fract->julia->c.w);
	printf("  Max Iterations: %d\n", data->fract->julia->max_iter);
	printf("  Step Size: %.6f\n", data->fract->step_size);
	printf("  Parameter Step: %.4f\n", data->param_step_size);
	
	printf("\x1b[36m[%s]\x1b[0m Rendering Settings:\n", __FILE__);
	printf("  Render Mode: %s\n", 
		   data->gl->render_mode == 0 ? "Wireframe" :
		   data->gl->render_mode == 1 ? "Solid" : "Colored");
	printf("  Auto Rotation: %s\n", data->gl->auto_rotate ? "ON" : "OFF");
	printf("  Zoom Factor: %.2fx\n", data->gl->zoom_factor);
	printf("  Triangles: %d\n", data->gl->num_tris);
	
	printf("\x1b[35m[%s]\x1b[0m Mathematical Enhancements:\n", __FILE__);
	const char *fractal_types[] = {"Julia Set", "Mandelbrot Set", "Hybrid"};
	const char *formula_names[] = {"Standard z²+c", "Cubic z³+c", "z²+z+c", "|z|²-z²+c"};
	printf("  Fractal Type: %s\n", fractal_types[data->fractal_type]);
	printf("  Quaternion Formula: %s\n", formula_names[data->quaternion_formula]);
	printf("  Deep Zoom Level: %.1fx\n", data->zoom_level);
	printf("  Double Precision: %s\n", data->use_double_precision ? "ON" : "OFF");
	printf("  Supersampling: %dx\n", data->supersampling);
	printf("  Adaptive Grid: %s\n", data->adaptive_grid ? "ON" : "OFF");
	if (data->adaptive_grid)
		printf("  Detail Threshold: %.2f\n", data->detail_threshold);
	
	printf("\x1b[33m[%s]\x1b[0m Controls:\n", __FILE__);
	printf("  Arrow Keys: Adjust Julia C.x/C.y\n");
	printf("  +/-: Adjust iterations\n");
	printf("  Q/A: Adjust parameter step size\n");
	printf("  Z/X: Zoom in/out\n");
	printf("  R: Toggle render mode\n");
	printf("  SPACE: Toggle auto-rotation\n");
	printf("  I: Toggle this info display\n");
	printf("  F: Force regeneration\n");
	printf("  T: Toggle fractal type\n");
	printf("  M: Toggle quaternion formula\n");
	printf("  P: Toggle double precision\n");
	printf("  O: Toggle supersampling\n");
	printf("  G/H: Deep zoom in/out\n");
	printf("  J: Toggle adaptive grid\n");
	printf("  K: Adjust detail threshold\n");
	printf("  ESC: Exit, S: Save\n");
	printf("\x1b[32m[%s]\x1b[0m ==========================================\n", __FILE__);
}

/**
 * @brief Regenerate fractal with current parameters
 * 
 * Performs complete fractal regeneration when parameters change,
 * including cleanup, recalculation, and buffer updates.
 */
void						regenerate_fractal(t_data *data)
{
	printf("\x1b[33m[%s]\x1b[0m Regenerating fractal...\n", __FILE__);
	double start_time = glfwGetTime();
	
	// Clean up existing calculation data
	clean_calcs(data);
	
	// Reset triangle pool for reuse
	reset_triangle_pool(data);
	data->flat_triangle_count = 0;
	
	// Recalculate point cloud with new parameters
	calculate_point_cloud(data);
	
	// Update graphics data
	gl_retrieve_tris(data);
	
	// Update VBO with new triangle data
	if (data->gl->tris && data->gl->num_pts > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->gl->vbo);
		glBufferData(GL_ARRAY_BUFFER, data->gl->num_pts * sizeof(float), 
					 (GLfloat *)data->gl->tris, GL_STATIC_DRAW);
		
		// Recalculate vertex normals for enhanced colored rendering
		calculate_vertex_normals(data);
		
		// Update normal buffer if enhanced shaders are available
		if (data->gl->enhanced_shader_program != 0 && data->gl->vertex_normals != NULL)
		{
			if (data->gl->normal_buffer == 0) {
				glGenBuffers(1, &data->gl->normal_buffer);
			}
			glBindBuffer(GL_ARRAY_BUFFER, data->gl->normal_buffer);
			glBufferData(GL_ARRAY_BUFFER, data->gl->num_pts * sizeof(float), 
						 data->gl->vertex_normals, GL_STATIC_DRAW);
		}
	}
	
	data->gl->needs_regeneration = 0;
	data->last_regen_time = glfwGetTime();
	
	double regen_time = data->last_regen_time - start_time;
	printf("\x1b[32m[%s]\x1b[0m Regeneration complete: %.2fs, %d triangles\n", 
		   __FILE__, regen_time, data->gl->num_tris);
	
	// Show updated parameters if info display is on
	if (data->show_info)
		print_parameter_info(data);
}
