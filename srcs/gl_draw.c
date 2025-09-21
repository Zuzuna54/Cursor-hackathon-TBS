#include "morphosis.h"

void 						run_graphics(t_gl *gl, float3 max, float3 min)
{
	gl_scale_tris(gl, max, min);

	init_gl(gl);
	createVAO(gl);
	createVBO(gl, gl->num_pts * sizeof(float), (GLfloat *)gl->tris);

	makeShaderProgram(gl);
	gl_set_attrib_ptr(gl, "pos", 3,3, 0);
	gl_calc_transforms(gl);
	
	// Set initial render mode
	handle_render_mode_change(gl);
	
	gl_render(gl);

	terminate_gl(gl);
}

void							gl_render(t_gl *gl)
{
	float 					time;
	float					delta;
	float 					old_time;

	old_time = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(gl->window))
	{
		processInput(gl->window, gl);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		time = (float)glfwGetTime();
		delta = (time - old_time);
		glm_mat4_identity(gl->matrix->model_mat);
		glm_rotate(gl->matrix->model_mat, (0.25f * delta * glm_rad(180.0f)), gl->matrix->up);
		glUniformMatrix4fv(gl->matrix->model, 1, GL_FALSE, (float *)gl->matrix->model_mat);
		old_time = time;
		glm_rotate(gl->matrix->view_mat, (0.25f * delta * glm_rad(180.0f)), gl->matrix->up);
		glUniformMatrix4fv(gl->matrix->view, 1, GL_FALSE, (float *)gl->matrix->view_mat);

		glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);

		glfwSwapBuffers(gl->window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

/**
 * @brief Enhanced graphics system with interactive parameter control
 * 
 * Extended version of run_graphics that supports real-time parameter
 * modification and fractal regeneration.
 */
void 						run_graphics_enhanced(t_data *data, float3 max, float3 min)
{
	t_gl *gl = data->gl;
	
	gl_scale_tris(gl, max, min);

	init_gl(gl);
	createVAO(gl);
	createVBO(gl, gl->num_pts * sizeof(float), (GLfloat *)gl->tris);

	makeShaderProgram(gl);
	gl_set_attrib_ptr(gl, "pos", 3,3, 0);
	gl_calc_transforms(gl);
	
	// Set initial render mode
	handle_render_mode_change(gl);
	
	// Set up mouse callbacks
	glfwSetWindowUserPointer(gl->window, gl);
	glfwSetCursorPosCallback(gl->window, mouse_callback);
	glfwSetMouseButtonCallback(gl->window, mouse_button_callback);
	glfwSetScrollCallback(gl->window, scroll_callback);
	
	// Initialize camera position
	update_camera_position(gl);
	
	// Print initial parameter info
	printf("\x1b[32m[%s]\x1b[0m Enhanced Morphosis started! Press 'I' for controls.\n", __FILE__);
	if (data->show_info)
		print_parameter_info(data);
	
	gl_render_enhanced(data);

	terminate_gl(gl);
}

/**
 * @brief Enhanced rendering loop with parameter control and regeneration
 * 
 * Main rendering loop that supports real-time parameter adjustment,
 * fractal regeneration, and enhanced rendering modes.
 */
void						gl_render_enhanced(t_data *data)
{
	t_gl *gl = data->gl;
	float 					time;
	float					delta;
	float 					old_time;

	// Initialize enhanced rendering system
	init_enhanced_rendering(gl);
	
	// Initialize timing for smooth rotation
	old_time = (float)glfwGetTime();
	
	// Set initial render mode
	handle_render_mode_change(gl);
	
	while (!glfwWindowShouldClose(gl->window))
	{
		// Enhanced input processing with parameter control
		processInput_enhanced(gl->window, gl, data);
		
		// Handle fractal regeneration if parameters changed
		if (gl->needs_regeneration)
		{
			regenerate_fractal(data);
			// Note: VBO is updated inside regenerate_fractal()
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update view matrix with camera position
		glm_lookat(gl->matrix->eye, gl->matrix->center, gl->matrix->up, gl->matrix->view_mat);
		glUniformMatrix4fv(gl->matrix->view, 1, GL_FALSE, (float *)gl->matrix->view_mat);
		
		// Handle auto-rotation
		if (gl->auto_rotate)
		{
			time = (float)glfwGetTime();
			delta = (time - old_time);
			if (delta > 0.0f) // Avoid division by zero and ensure forward time
			{
				// Apply rotation delta to existing model matrix (accumulate rotation)
				glm_rotate(gl->matrix->model_mat, (0.5f * delta * glm_rad(90.0f)), gl->matrix->up);
				old_time = time;
			}
		}
		
		// Always update the model matrix uniform (whether rotating or not)
		glUniformMatrix4fv(gl->matrix->model, 1, GL_FALSE, (float *)gl->matrix->model_mat);

		// Render triangles
		if (gl->num_pts > 0)
		{
			if (gl->render_mode == 2 && gl->use_enhanced_rendering)
			{
				// Use enhanced rendering for colored mode
				render_enhanced(data);
			}
			else
			{
				// Use standard rendering for wireframe and solid modes
				glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);
			}
		}

		glfwSwapBuffers(gl->window);
		glfwPollEvents();
	}
}
