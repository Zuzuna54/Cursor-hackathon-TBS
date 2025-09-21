#include "morphosis.h"

t_fract						*init_fract(void)
{
	t_fract 				*fract;

	if (!(fract = (t_fract *)malloc(sizeof(t_fract))))
		error(MALLOC_FAIL_ERR, NULL);

	fract->p0.x = -1.5f;
	fract->p0.y = -1.5f;
	fract->p0.z = -1.5f;

	fract->p1.x = 1.5f;
	fract->p1.y = 1.5f;
	fract->p1.z = 1.5f;

	fract->grid_length = 3.0f;
	fract->grid_size = 0.0f;

	fract->grid.x = NULL;
	fract->grid.y = NULL;
	fract->grid.z = NULL;

	fract->step_size = 0.05f;

	fract->julia = init_julia();
	return fract;
}

t_julia 					*init_julia(void)
{
	t_julia 				*julia;

	if (!(julia = (t_julia *)malloc(sizeof(t_julia))))
		error(MALLOC_FAIL_ERR, NULL);

	julia->max_iter = 6;
	julia->threshold = 2.0f;
	julia->w = 0.0f;

	julia->c.x = -0.2f;
	julia->c.y = 0.8f;
	julia->c.z = 0.0f;
	julia->c.w = 0.0f;

	return julia;
}

t_data						*init_data(void)
{
	t_data 					*data;

	if (!(data = (t_data *)malloc(sizeof(t_data))))
		error(MALLOC_FAIL_ERR, NULL);
	data->gl = init_gl_struct();
	data->fract = init_fract();
	data->vertexpos = NULL;
	data->vertexval = NULL;
	data->triangles = NULL;
	
	// Initialize memory optimization fields
	data->triangle_pool = NULL;
	data->triangle_pool_size = 0;
	data->triangle_pool_used = 0;
	
	// Initialize cache-friendly triangle storage
	data->flat_triangles = NULL;
	data->flat_triangle_count = 0;
	data->flat_triangle_capacity = 0;
	
	// Initialize marching cubes optimization
	data->mc_vertlist = NULL;
	data->mc_vertlist_size = 12; // Always 12 vertices for cube edges
	
	// Initialize interactive parameter control
	data->param_step_size = 0.01f;	// Default parameter adjustment step
	data->show_info = 1;			// Show info by default
	data->last_regen_time = 0.0;
	
	// Initialize deep zoom and mathematical enhancements
	data->zoom_level = 1.0;			// Start at 1x zoom
	data->adaptive_grid = 0;		// Disabled by default
	data->max_grid_depth = 3;		// Maximum 3 levels of refinement
	data->detail_threshold = 0.1f;	// Threshold for detail detection
	data->use_double_precision = 0;	// Use float by default
	
	// Initialize alternative fractal support
	data->fractal_type = 0;			// Julia set by default
	data->quaternion_formula = 0;	// Standard formula
	
	// Initialize advanced sampling
	data->supersampling = 1;		// No anti-aliasing by default
	data->adaptive_sampling = 0;	// Disabled by default
	data->progressive_refinement = 0; // Disabled by default
	
	return data;
}

void						init_vertex(t_data *data)
{
	size_t 					size;

	size = pow(data->fract->grid_size, 3) * 8;
	if (!(data->vertexpos = (float3 *)malloc(size * sizeof(float3))))
		error(MALLOC_FAIL_ERR, data);
	if (!(data->vertexval = (float *)malloc(size * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
}

void						init_grid(t_data *data)
{
	t_fract 				*f;

	f = data->fract;
	if (!(f->grid.x = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
	if (!(f->grid.y = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
	if (!(f->grid.z = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
}
