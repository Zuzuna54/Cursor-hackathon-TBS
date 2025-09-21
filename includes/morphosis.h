#ifndef _MORPHOSIS_H
# define _MORPHOSIS_H

# include "stdio.h"
# include "stdlib.h"
# include "ctype.h"
# include "string.h"

# include <gl_includes.h>
# include <errors.h>
# include <obj.h>
# include <matrix.h>

# define OUTPUT_FILE "./fractal.obj"
# define OUTPUT_PRECISION 3

t_data						*init_data(void);
t_gl						*init_gl_struct(void);
t_julia 					*init_julia(void);
t_fract						*init_fract(void);
void						init_grid(t_data *data);
void						init_vertex(t_data *data);

void 						error(int errno, t_data *data);
float						s_size_warning(float size);

float3						**arr_float3_cat(float3 **f_from, float3 **f_to, uint2 *len);
float3 						**alloc_float3_arr(float3 **mem, uint2 *len);

// Memory optimization functions
void						init_triangle_pool(t_data *data);
float3						*get_triangle_from_pool(t_data *data);
void						reset_triangle_pool(t_data *data);
void						clean_triangle_pool(t_data *data);

// Cache-friendly triangle storage
void						init_flat_triangles(t_data *data);
void						add_triangle_to_flat(t_data *data, float3 *vertices);
void						clean_flat_triangles(t_data *data);

// Optimized marching cubes
float3						**polygonise_optimized(float3 *v_pos, float *v_val, uint2 *pos, t_data *data);

// Graphics pipeline optimizations
void						createVBO_optimized(t_gl *gl, GLsizeiptr size, GLfloat *points);
void						updateVBO_optimized(t_gl *gl, GLsizeiptr size, GLfloat *points);

// Enhanced rendering features
void						processInput_enhanced(GLFWwindow *window, t_gl *gl, t_data *data);
void						regenerate_fractal(t_data *data);
void						print_parameter_info(t_data *data);
void						handle_render_mode_change(t_gl *gl);
void						handle_camera_controls(GLFWwindow *window, t_gl *gl);
void						update_camera_position(t_gl *gl);

// Mathematical enhancements (Task 3.2)
// Deep zoom and precision
double						cl_quat_mod_d(cl_quat_d q);
cl_quat_d					cl_quat_mult_d(cl_quat_d q1, cl_quat_d q2);
cl_quat_d					cl_quat_sum_d(cl_quat_d q1, cl_quat_d q2);
float						sample_4D_Julia_deep_zoom(t_julia *julia, double3 pos, double zoom_level);

// Alternative fractal types
float						sample_4D_Mandelbrot(t_julia *julia, float3 pos);
float						sample_4D_Julia_alternative_formula(t_julia *julia, float3 pos, int formula);

// Advanced sampling techniques
int							should_refine_grid_cell(t_data *data, float3 center, float cell_size, int current_depth);
float						sample_with_supersampling(t_data *data, float3 pos);
float						sample_fractal_enhanced(t_data *data, float3 pos);

void 						clean_up(t_data *data);
void						clean_gl(t_gl *gl);
void 						clean_fract(t_fract *fract);
void						clean_trigs(float3 **trigs, uint len);
void 						clean_calcs(t_data *data);

void 						calculate_point_cloud(t_data *data);
void						create_grid(t_data *data);
void 						subdiv_grid(float start, float stop, float step, float *axis);
void						define_voxel(t_fract *fract, float s);

void						build_fractal(t_data *data);

float 						sample_4D_Julia(t_julia *julia, float3 pos);

// Optimized Julia set functions
float						sample_4D_Julia_optimized(t_julia *julia, float3 pos);
float						cl_quat_mod_fast(cl_quat q);

float3 						**polygonise(float3 *v_pos, float *v_val, uint2 *pos, t_data *data);

void 						export_obj(t_data *data);
void						write_mesh(t_data *data, int surface, obj *o);

// Enhanced rendering functions
void						init_enhanced_rendering(t_gl *gl);
void						cleanup_enhanced_rendering(t_gl *gl);
void						calculate_triangle_normals(float3 *triangles, int num_triangles, t_vertex_enhanced *enhanced_vertices);
void						generate_triangle_colors(float3 *triangles, int num_triangles, t_vertex_enhanced *enhanced_vertices, t_data *data);
void						update_enhanced_vertices(t_data *data);
void						render_enhanced(t_data *data);
GLuint						compile_enhanced_shaders(void);
float3						calculate_face_normal(float3 v1, float3 v2, float3 v3);

#endif
