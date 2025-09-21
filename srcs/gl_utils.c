#include "morphosis.h"

void 						framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	int 				i = 0;
	glViewport(0, 0, width, height);

	if (window)
		i++;
}

void 						processInput(GLFWwindow *window, t_gl *gl)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		gl->export = 1;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

/**
 * @brief Enhanced input processing with dynamic parameter control
 * 
 * Keyboard Controls:
 * - ESC: Exit application
 * - S: Save/Export fractal
 * - R: Toggle render mode (wireframe/solid/colored)
 * - SPACE: Toggle auto-rotation
 * - I: Toggle info display
 * - +/-: Adjust iterations
 * - Arrow keys: Adjust Julia set parameters
 * - Q/A: Adjust step size
 * - Z/X: Zoom in/out
 * - F: Regenerate fractal
 */
void 						processInput_enhanced(GLFWwindow *window, t_gl *gl, t_data *data)
{
	static double last_key_time = 0.0;
	double current_time = glfwGetTime();
	static int r_pressed = 0, space_pressed = 0, i_pressed = 0;
	static int f_pressed = 0;
	
	// Prevent key repeat spam (100ms delay)
	if (current_time - last_key_time < 0.1)
		return;

	// Basic controls
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		gl->export = 1;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	// Enhanced rendering controls
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !r_pressed)
	{
		gl->render_mode = (gl->render_mode + 1) % 3; // Cycle through modes
		handle_render_mode_change(gl);
		printf("\x1b[36m[%s]\x1b[0m Render mode: %s\n", __FILE__, 
			   gl->render_mode == 0 ? "Wireframe" : 
			   gl->render_mode == 1 ? "Solid" : "Colored");
		r_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) r_pressed = 0;
	
	// Auto-rotation toggle
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed)
	{
		gl->auto_rotate = !gl->auto_rotate;
		printf("\x1b[36m[%s]\x1b[0m Auto-rotation: %s\n", __FILE__, 
			   gl->auto_rotate ? "ON" : "OFF");
		space_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) space_pressed = 0;
	
	// Info display toggle
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !i_pressed)
	{
		data->show_info = !data->show_info;
		printf("\x1b[36m[%s]\x1b[0m Info display: %s\n", __FILE__, 
			   data->show_info ? "ON" : "OFF");
		if (data->show_info) print_parameter_info(data);
		i_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) i_pressed = 0;
	
	// Parameter adjustments
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || 
		glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) // + key
	{
		data->fract->julia->max_iter += 1;
		if (data->fract->julia->max_iter > 50) data->fract->julia->max_iter = 50;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Iterations: %d\n", __FILE__, data->fract->julia->max_iter);
		last_key_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) // - key
	{
		data->fract->julia->max_iter -= 1;
		if (data->fract->julia->max_iter < 1) data->fract->julia->max_iter = 1;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Iterations: %d\n", __FILE__, data->fract->julia->max_iter);
		last_key_time = current_time;
	}
	
	// Julia set parameter adjustments (arrow keys)
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		data->fract->julia->c.x += data->param_step_size;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Julia C.x: %.3f\n", __FILE__, data->fract->julia->c.x);
		last_key_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		data->fract->julia->c.x -= data->param_step_size;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Julia C.x: %.3f\n", __FILE__, data->fract->julia->c.x);
		last_key_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		data->fract->julia->c.y += data->param_step_size;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Julia C.y: %.3f\n", __FILE__, data->fract->julia->c.y);
		last_key_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		data->fract->julia->c.y -= data->param_step_size;
		gl->needs_regeneration = 1;
		printf("\x1b[33m[%s]\x1b[0m Julia C.y: %.3f\n", __FILE__, data->fract->julia->c.y);
		last_key_time = current_time;
	}
	
	// Step size adjustment
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		data->param_step_size *= 1.5f;
		if (data->param_step_size > 0.1f) data->param_step_size = 0.1f;
		printf("\x1b[36m[%s]\x1b[0m Step size: %.4f\n", __FILE__, data->param_step_size);
		last_key_time = current_time;
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		data->param_step_size /= 1.5f;
		if (data->param_step_size < 0.0001f) data->param_step_size = 0.0001f;
		printf("\x1b[36m[%s]\x1b[0m Step size: %.4f\n", __FILE__, data->param_step_size);
		last_key_time = current_time;
	}
	
	// Camera controls
	handle_camera_controls(window, gl);
	
	// Force regeneration
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !f_pressed)
	{
		gl->needs_regeneration = 1;
		printf("\x1b[32m[%s]\x1b[0m Force regeneration requested\n", __FILE__);
		f_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) f_pressed = 0;
	
	// Mathematical enhancement controls
	static int t_pressed = 0, m_pressed = 0, p_pressed = 0, o_pressed = 0;
	static int g_pressed = 0, h_pressed = 0, j_pressed = 0, k_pressed = 0;
	
	// Toggle fractal type (T key)
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !t_pressed)
	{
		data->fractal_type = (data->fractal_type + 1) % 3;
		const char *type_names[] = {"Julia Set", "Mandelbrot Set", "Hybrid"};
		printf("\x1b[35m[%s]\x1b[0m Fractal Type: %s\n", __FILE__, type_names[data->fractal_type]);
		gl->needs_regeneration = 1;
		t_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) t_pressed = 0;
	
	// Toggle quaternion formula (M key)
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !m_pressed)
	{
		data->quaternion_formula = (data->quaternion_formula + 1) % 4;
		const char *formula_names[] = {"Standard z²+c", "Cubic z³+c", "z²+z+c", "|z|²-z²+c"};
		printf("\x1b[35m[%s]\x1b[0m Quaternion Formula: %s\n", __FILE__, formula_names[data->quaternion_formula]);
		gl->needs_regeneration = 1;
		m_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) m_pressed = 0;
	
	// Toggle double precision (P key)
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !p_pressed)
	{
		data->use_double_precision = !data->use_double_precision;
		printf("\x1b[35m[%s]\x1b[0m Double Precision: %s\n", __FILE__, 
			   data->use_double_precision ? "ON" : "OFF");
		gl->needs_regeneration = 1;
		p_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) p_pressed = 0;
	
	// Toggle supersampling (O key)
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !o_pressed)
	{
		data->supersampling++;
		if (data->supersampling > 3) data->supersampling = 1;
		printf("\x1b[35m[%s]\x1b[0m Supersampling: %dx\n", __FILE__, data->supersampling);
		if (data->supersampling > 1)
			printf("\x1b[33m[%s]\x1b[0m Warning: Supersampling will slow regeneration\n", __FILE__);
		gl->needs_regeneration = 1;
		o_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) o_pressed = 0;
	
	// Zoom level controls (G/H keys)
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !g_pressed)
	{
		data->zoom_level *= 2.0;
		if (data->zoom_level > 1000000.0) data->zoom_level = 1000000.0;
		printf("\x1b[35m[%s]\x1b[0m Zoom Level: %.1fx\n", __FILE__, data->zoom_level);
		if (data->zoom_level > 1000.0)
			printf("\x1b[33m[%s]\x1b[0m Deep zoom active - consider enabling double precision (P)\n", __FILE__);
		gl->needs_regeneration = 1;
		g_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) g_pressed = 0;
	
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !h_pressed)
	{
		data->zoom_level /= 2.0;
		if (data->zoom_level < 1.0) data->zoom_level = 1.0;
		printf("\x1b[35m[%s]\x1b[0m Zoom Level: %.1fx\n", __FILE__, data->zoom_level);
		gl->needs_regeneration = 1;
		h_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) h_pressed = 0;
	
	// Adaptive grid controls (J/K keys)
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !j_pressed)
	{
		data->adaptive_grid = !data->adaptive_grid;
		printf("\x1b[35m[%s]\x1b[0m Adaptive Grid: %s\n", __FILE__, 
			   data->adaptive_grid ? "ON" : "OFF");
		if (data->adaptive_grid)
			printf("\x1b[33m[%s]\x1b[0m Note: Adaptive grid is experimental and may slow generation\n", __FILE__);
		gl->needs_regeneration = 1;
		j_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) j_pressed = 0;
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !k_pressed)
	{
		data->detail_threshold += 0.05f;
		if (data->detail_threshold > 0.5f) data->detail_threshold = 0.05f;
		printf("\x1b[35m[%s]\x1b[0m Detail Threshold: %.2f\n", __FILE__, data->detail_threshold);
		if (data->adaptive_grid)
			gl->needs_regeneration = 1;
		k_pressed = 1;
		last_key_time = current_time;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) k_pressed = 0;
}

void 						init_gl(t_gl *gl)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	gl->window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Morphosis", NULL, NULL);
	glfwMakeContextCurrent(gl->window);
	glewExperimental = GL_TRUE;
	glewInit();
	if (!gl->window)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit (1);
	}
	glfwSetFramebufferSizeCallback(gl->window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
}

void 						terminate_gl(t_gl *gl)
{
	glDeleteVertexArrays(1, &gl->vao);
	glDeleteBuffers(1, &gl->vbo);
	glDeleteProgram(gl->shaderProgram);
	glfwTerminate();
}

/**
 * @brief Mouse movement callback for camera rotation
 * 
 * Handles mouse movement for camera rotation when dragging.
 */
void 						mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);
	if (!gl || !gl->mouse_dragging) return;
	
	double xoffset = xpos - gl->last_mouse_x;
	double yoffset = gl->last_mouse_y - ypos; // Reversed since y-coordinates go from bottom to top
	
	gl->last_mouse_x = xpos;
	gl->last_mouse_y = ypos;
	
	float sensitivity = 0.005f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	gl->camera_rotation_y += (float)xoffset;
	gl->camera_rotation_x += (float)yoffset;
	
	// Constrain pitch to avoid gimbal lock
	if (gl->camera_rotation_x > 1.5f) gl->camera_rotation_x = 1.5f;
	if (gl->camera_rotation_x < -1.5f) gl->camera_rotation_x = -1.5f;
	
	update_camera_position(gl);
}

/**
 * @brief Mouse button callback for drag control
 * 
 * Handles mouse button press/release for camera drag control.
 */
void 						mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);
	if (!gl) return;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			gl->mouse_dragging = 1;
			glfwGetCursorPos(window, &gl->last_mouse_x, &gl->last_mouse_y);
		}
		else if (action == GLFW_RELEASE)
		{
			gl->mouse_dragging = 0;
		}
	}
	
	// Suppress unused parameter warnings
	(void)mods;
}

/**
 * @brief Mouse scroll callback for zoom control
 * 
 * Handles mouse scroll wheel for smooth zoom control.
 */
void 						scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	t_gl *gl = (t_gl *)glfwGetWindowUserPointer(window);
	if (!gl) return;
	
	float zoom_speed = 0.1f;
	
	if (yoffset > 0) // Scroll up - zoom in
	{
		gl->zoom_factor *= (1.0f + zoom_speed);
		if (gl->zoom_factor > 5.0f) gl->zoom_factor = 5.0f;
	}
	else if (yoffset < 0) // Scroll down - zoom out
	{
		gl->zoom_factor *= (1.0f - zoom_speed);
		if (gl->zoom_factor < 0.1f) gl->zoom_factor = 0.1f;
	}
	
	update_camera_position(gl);
	
	// Suppress unused parameter warning
	(void)xoffset;
}
