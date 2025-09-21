#include "morphosis.h"

void						build_fractal(t_data *data)
{
	t_fract 				*f;
	float3 					**new_tris;
	size_t 					i;
	uint2					pos;

	i = 0;
	f = data->fract;
	data->len.x = 0;
	data->len.y = 0;
	pos.x = 0;
	pos.y = 0;
	new_tris = NULL;
	
	for (size_t z = 0; z < f->grid_size; z++)
	{
		printf("%zu/%.0f\n", (z + 1), f->grid_size);
        for (size_t y = 0; y < f->grid_size; y++)
		{
            for (size_t x = 0; x < f->grid_size; x++)
			{
                for (int c = 0; c < 8; c++)
				{
					data->vertexpos[i].x = f->grid.x[x] + f->voxel[c].dx;
					data->vertexpos[i].y = f->grid.y[y] + f->voxel[c].dy;
					data->vertexpos[i].z = f->grid.z[z] + f->voxel[c].dz;
					// Use enhanced fractal sampling with all mathematical improvements
					data->vertexval[i] = sample_fractal_enhanced(data, data->vertexpos[i]);
					i++;
				}
				pos.y += 8;
				new_tris = polygonise_optimized(data->vertexpos, data->vertexval, &pos, data);
				if (new_tris)
				{
					if (!(data->triangles = arr_float3_cat(new_tris, data->triangles, &data->len)))
						error(MALLOC_FAIL_ERR, data);
				}
				pos.x = pos.y;
			}
		}
	}
	data->gl->num_tris = data->len.x;
	data->gl->num_pts = data->len.x * 3 * 3;
}
