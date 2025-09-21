#ifndef _MORPHOSIS_GL_INCLUDES_H
# define _MORPHOSIS_GL_INCLUDES_H

# define SHADER_SRC_SIZE 10000

# define VERTEX_SRC "./shaders/vertex.shader"
# define FRAGMENT_SRC "./shaders/fragment.shader"

# define GLEW_STATIC
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include "stb_image.h"
# include <cglm/cglm.h>

# include <structures.h>

# define SRC_WIDTH 800
# define SRC_HEIGHT 600


void 						init_gl(t_gl *gl);
t_matrix 					*initGlMatrices(void);

void 						run_graphics(t_gl *gl, float3 max, float3 min);
void 						gl_render(t_gl *gl);

// Enhanced rendering system
void 						run_graphics_enhanced(t_data *data, float3 max, float3 min);
void 						gl_render_enhanced(t_data *data);

void 						framebuffer_size_callback(GLFWwindow *window, int width, int height);
void 						processInput(GLFWwindow *window, t_gl *gl);
void 						terminate_gl(t_gl *gl);

// Mouse callback functions
void 						mouse_callback(GLFWwindow *window, double xpos, double ypos);
void 						mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void 						scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void 						createVBO(t_gl *gl, GLsizeiptr size, GLfloat *points);
void						createVAO(t_gl *gl);

void 						makeShaderProgram(t_gl *gl);
char						*readShaderSource(char *src_name);
GLuint 						createShader(GLenum type, char **src);

void 						createProgram(t_gl *gl);

void						gl_set_attrib_ptr(t_gl *gl, char *attrib_name, GLint num_vals, int stride, int offset);
void						gl_retrieve_tris(t_data *data);

void 						gl_calc_transforms(t_gl *gl);
void						gl_scale_tris(t_gl *gl, float3 max, float3 min);

#endif
