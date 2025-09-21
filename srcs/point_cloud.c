#include "morphosis.h"

void 						calculate_point_cloud(t_data *data)
{
	t_fract 				*fract;

	fract = data->fract;
	fract->grid_size = fract->grid_length / fract->step_size;
	init_grid(data);
	init_vertex(data);
	
	// Initialize memory optimizations after we know the grid size
	init_triangle_pool(data);
	init_flat_triangles(data);
	
	// Initialize marching cubes vertex list (small, fixed size allocation)
	printf("\x1b[36m[%s]\x1b[0m Allocating marching cubes vertex list: %u vertices\n", 
		   __FILE__, data->mc_vertlist_size);
	if (!(data->mc_vertlist = (float3 *)malloc(data->mc_vertlist_size * sizeof(float3))))
		error(MALLOC_FAIL_ERR, data);
	
	create_grid(data);
	define_voxel(fract, fract->step_size);

	build_fractal(data);
}

void						create_grid(t_data *data)
{
	subdiv_grid(data->fract->p0.x, data->fract->p1.x, data->fract->step_size, data->fract->grid.x);
	subdiv_grid(data->fract->p0.y, data->fract->p1.y, data->fract->step_size, data->fract->grid.y);
	subdiv_grid(data->fract->p0.z, data->fract->p1.z, data->fract->step_size, data->fract->grid.z);
}

void 						subdiv_grid(float start, float stop, float step, float *axis)
{
	float 					val;
	int						i;

	val = start;
	i = 0;
	while (val <= stop)
	{
		axis[i++] = val;
		val += step;
	}
}

void						define_voxel(t_fract *fract, float s)
{
	const float 			zz[2] = {-s / 2, s / 2};
	const float 			xx[4] = {-s / 2, s / 2, s / 2, -s / 2};
	const float 			yy[4] = {s / 2, s / 2, -s / 2, -s / 2};
	unsigned 				n = 0;

	for (unsigned i = 0; i < 2; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			fract->voxel[n].dx = xx[j];
			fract->voxel[n].dy = yy[j];
			fract->voxel[n].dz = zz[i];
			n++;
		}
	}
}
