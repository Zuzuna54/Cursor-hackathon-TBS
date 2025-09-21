#include "morphosis.h"

void 						clean_calcs(t_data *data)
{
	if (data->vertexpos && data->vertexval)
	{
		free(data->vertexval);
		free(data->vertexpos);
		data->vertexval = NULL;
		data->vertexpos = NULL;
	}
}

void 						clean_fract(t_fract *fract)
{
	if (!fract)
		return;
	if (fract->julia)
		free(fract->julia);
	if (fract->grid.x)
		free(fract->grid.x);
	if (fract->grid.y)
		free(fract->grid.y);
	if (fract->grid.z)
		free(fract->grid.z);
	free(fract);
}

void						clean_gl(t_gl *gl)
{
	if (gl->matrix)
		free(gl->matrix);
	if (gl->tris)
		free(gl->tris);
	
	// Clean up enhanced rendering resources
	cleanup_enhanced_rendering(gl);
	
	free(gl);
}

void						clean_trigs(float3 **trigs, uint len)
{
	for (uint i = 0; i < len; i++)
		free(trigs[i]);
	free(trigs);
}

void 						clean_up(t_data *data)
{
	if (data)
	{
		if (data->gl)
			clean_gl(data->gl);
		if (data->fract)
			clean_fract(data->fract);
		if (data->vertexpos)
			free(data->vertexpos);
		if (data->vertexval)
			free(data->vertexval);
		if (data->triangles)
			clean_trigs(data->triangles, data->len.x);
		
		// Clean up memory optimization structures
		clean_triangle_pool(data);
		clean_flat_triangles(data);
		
		// Clean up marching cubes vertex list
		if (data->mc_vertlist) {
			free(data->mc_vertlist);
			data->mc_vertlist = NULL;
		}
		
		free(data);
	}
}
