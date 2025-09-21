#pragma once

# include <lib_complex.h>

typedef struct 				s_matrix
{
	mat4 					model_mat;
	mat4 					projection_mat;
	mat4 					view_mat;

	vec3 					eye;
	vec3 					center;
	vec3 					up;

	GLuint 					model;
	GLuint 					view;
}							t_matrix;

typedef struct 				s_vertex_enhanced
{
	float3					position;	// Vertex position (x, y, z)
	float3					normal;		// Surface normal (x, y, z)
	float3					color;		// Vertex color (r, g, b)
}							t_vertex_enhanced;

typedef struct 				s_gl
{
	GLFWwindow 				*window;

	int						export;

	GLuint 					vertexShader;
	GLuint 					fragmentShader;
	GLuint					shaderProgram;

	GLuint 					vbo;
	GLuint 					vao;

	float					*tris;
	uint 					num_pts;
	uint					num_tris;
	t_matrix 				*matrix;
	
	// Enhanced rendering features
	int						render_mode;		// 0=wireframe, 1=solid, 2=colored
	int						needs_regeneration;	// Flag for parameter changes
	float					zoom_factor;		// Camera zoom level
	int						auto_rotate;		// Enable/disable auto rotation
	
	// Mouse camera controls
	double					last_mouse_x;		// Last mouse X position
	double					last_mouse_y;		// Last mouse Y position
	int						mouse_dragging;		// Mouse drag state
	float					camera_rotation_x;	// Camera rotation around X axis
	float					camera_rotation_y;	// Camera rotation around Y axis
	
	// Enhanced rendering support
	t_vertex_enhanced		*enhanced_vertices;	// Enhanced vertex buffer with normals/colors
	GLuint					enhanced_vbo;		// Enhanced VBO handle
	GLuint					enhanced_program;	// Enhanced shader program
	GLint					enhanced_uniforms[10]; // Enhanced shader uniform locations
	int						use_enhanced_rendering; // Flag to enable enhanced rendering
}							t_gl;

typedef struct 				s_julia
{
	uint					max_iter;
	float 					threshold;
	float 					w;
	cl_quat 				c;
}							t_julia;

typedef struct 				s_grid
{
	float 					*x;
	float 					*y;
	float 					*z;
}							t_grid;

typedef struct 				s_voxel
{
	float 					dx;
	float 					dy;
	float 					dz;
}							t_voxel;

typedef struct				s_fract
{
	float3 					p0;
	float3 					p1;
	float 					step_size;
	float 					grid_length;
	float 					grid_size;

	t_julia 				*julia;
	t_grid 					grid;
	t_voxel 				voxel[8];
}							t_fract;

typedef struct 				s_data
{
	t_gl					*gl;
	t_fract 				*fract;
	float3 					*vertexpos;
	float					*vertexval;
	float3					**triangles;

	uint2 					len;
	
	// Memory optimization: pre-allocated triangle storage
	float3					*triangle_pool;		// Pre-allocated triangle vertex pool
	uint					triangle_pool_size;	// Size of pre-allocated pool
	uint					triangle_pool_used;	// Current usage in pool
	
	// Cache-friendly triangle storage
	float3					*flat_triangles;	// Flat array of triangle vertices
	uint					flat_triangle_count;// Number of triangles in flat array
	uint					flat_triangle_capacity; // Capacity of flat array
	
	// Marching cubes optimization: pre-allocated vertex list
	float3					*mc_vertlist;		// Pre-allocated vertex list (12 vertices max)
	uint					mc_vertlist_size;	// Size of vertex list (always 12)
	
	// Interactive parameter control
	float					param_step_size;	// Step size for parameter adjustments
	int						show_info;			// Display parameter information
	double					last_regen_time;	// Time of last regeneration
	
	// Deep zoom and mathematical enhancements
	double					zoom_level;			// Current zoom level (for precision scaling)
	int						adaptive_grid;		// Enable adaptive grid refinement
	int						max_grid_depth;		// Maximum refinement depth
	float					detail_threshold;	// Threshold for detail detection
	int						use_double_precision; // Use double precision for deep zoom
	
	// Alternative fractal support
	int						fractal_type;		// 0=Julia, 1=Mandelbrot, 2=Hybrid
	int						quaternion_formula;	// Different quaternion iteration formulas
	
	// Advanced sampling
	int						supersampling;		// Anti-aliasing level (1=off, 2-4=samples)
	int						adaptive_sampling;	// Enable complexity-based sampling
	int						progressive_refinement; // Enable progressive detail enhancement
}							t_data;
