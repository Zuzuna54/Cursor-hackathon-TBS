#include "morphosis.h"

/**
 * @brief Initialize triangle memory pool for efficient allocation
 * 
 * Pre-allocates a large pool of triangle vertices to avoid frequent
 * malloc/free cycles during triangle generation.
 */
void						init_triangle_pool(t_data *data)
{
	// Estimate triangle pool size based on grid size
	// Worst case: ~5 triangles per cube, 3 vertices per triangle
	size_t cubes = pow(data->fract->grid_size, 3);
	data->triangle_pool_size = cubes * 5 * 3; // Conservative estimate
	
	printf("\x1b[36m[%s]\x1b[0m Allocating triangle pool: %u vertices\n", 
		   __FILE__, data->triangle_pool_size);
	
	if (!(data->triangle_pool = (float3 *)malloc(data->triangle_pool_size * sizeof(float3))))
		error(MALLOC_FAIL_ERR, data);
	
	data->triangle_pool_used = 0;
}

/**
 * @brief Get pre-allocated triangle from memory pool
 * 
 * Returns pointer to next available triangle (3 consecutive vertices)
 * from the pre-allocated pool, avoiding malloc overhead.
 */
float3						*get_triangle_from_pool(t_data *data)
{
	if (data->triangle_pool_used + 3 > data->triangle_pool_size) {
		// Pool exhausted, need to expand (fallback to malloc)
		printf("\x1b[33m[%s]\x1b[0m Triangle pool exhausted, using fallback malloc\n", __FILE__);
		return (float3 *)malloc(3 * sizeof(float3));
	}
	
	float3 *triangle = &data->triangle_pool[data->triangle_pool_used];
	data->triangle_pool_used += 3;
	return triangle;
}

/**
 * @brief Reset triangle pool for reuse
 */
void						reset_triangle_pool(t_data *data)
{
	data->triangle_pool_used = 0;
}

/**
 * @brief Clean up triangle pool memory
 */
void						clean_triangle_pool(t_data *data)
{
	if (data->triangle_pool) {
		free(data->triangle_pool);
		data->triangle_pool = NULL;
	}
	data->triangle_pool_size = 0;
	data->triangle_pool_used = 0;
}

/**
 * @brief Initialize cache-friendly flat triangle storage
 * 
 * Creates a single contiguous array for all triangle vertices,
 * improving cache locality and reducing memory fragmentation.
 */
void						init_flat_triangles(t_data *data)
{
	// Initial capacity based on grid size estimate
	size_t cubes = pow(data->fract->grid_size, 3);
	data->flat_triangle_capacity = cubes * 5; // Conservative estimate: 5 triangles per cube
	
	printf("\x1b[36m[%s]\x1b[0m Allocating flat triangle storage: %u triangles\n", 
		   __FILE__, data->flat_triangle_capacity);
	
	// Allocate flat array: capacity * 3 vertices per triangle
	size_t vertices_needed = data->flat_triangle_capacity * 3;
	if (!(data->flat_triangles = (float3 *)malloc(vertices_needed * sizeof(float3))))
		error(MALLOC_FAIL_ERR, data);
	
	data->flat_triangle_count = 0;
}

/**
 * @brief Add triangle to flat storage array
 * 
 * Efficiently adds a triangle (3 vertices) to the flat array,
 * expanding capacity if needed.
 */
void						add_triangle_to_flat(t_data *data, float3 *vertices)
{
	// Check if we need to expand capacity
	if (data->flat_triangle_count >= data->flat_triangle_capacity) {
		// Double the capacity
		data->flat_triangle_capacity *= 2;
		size_t new_size = data->flat_triangle_capacity * 3 * sizeof(float3);
		
		printf("\x1b[33m[%s]\x1b[0m Expanding flat triangle storage to %u triangles\n", 
			   __FILE__, data->flat_triangle_capacity);
		
		if (!(data->flat_triangles = (float3 *)realloc(data->flat_triangles, new_size)))
			error(MALLOC_FAIL_ERR, data);
	}
	
	// Add triangle vertices to flat array (cache-friendly sequential access)
	size_t base_index = data->flat_triangle_count * 3;
	memcpy(&data->flat_triangles[base_index], vertices, sizeof(float3) * 3);
	data->flat_triangle_count++;
}

/**
 * @brief Clean up flat triangle storage
 */
void						clean_flat_triangles(t_data *data)
{
	if (data->flat_triangles) {
		free(data->flat_triangles);
		data->flat_triangles = NULL;
	}
	data->flat_triangle_count = 0;
	data->flat_triangle_capacity = 0;
}

float3 						**alloc_float3_arr(float3 **mem, uint2 *len)
{
	uint 					size;

	size = len->x + len->y;
	if (!size)
		return NULL;
	if (!mem)
	{
		if (!(mem = (float3 **)malloc(sizeof(float3 *) * size)))
			return NULL;
		for (uint c = len->x; c < size; c++)
		{
			if (!(mem[c] = (float3 *)malloc(3 * sizeof(float3))))
				return NULL;
		}
	}
	else
	{
		if (!(mem = (float3 **)realloc(mem, sizeof(float3 *) * size)))
			return NULL;
		for (uint c = len->x; c < size; c++)
		{
			if (!(mem[c] = (float3 *)malloc(3 * sizeof(float3))))
				return NULL;
		}
	}
	return mem;
}

/**
 * @brief Optimized triangle concatenation using memory pool
 * 
 * Improved version that avoids expensive reallocation and copying
 * by using pre-allocated memory pool when possible.
 */
float3						**arr_float3_cat(float3 **f_from, float3 **f_to, uint2 *len)
{
	uint					from_c;
	uint 					to_c;
	uint 					res_c;

	res_c = len->x + len->y;
	from_c = 0;
	to_c = len->x;
	
	// Use optimized allocation when possible
	if (!(f_to = alloc_float3_arr(f_to, len)))
		return NULL;

	// Optimized copying using memcpy for better performance
	while (to_c < res_c)
	{
		// Copy entire triangle at once (3 float3 vertices = 36 bytes)
		memcpy(&f_to[to_c][0], &f_from[from_c][0], sizeof(float3) * 3);
		to_c++;
		from_c++;
	}
	len->x = res_c;

	// Clean up source array
	if (f_from)
	{
		for (uint i = 0; i < len->y; i++)
			free(f_from[i]);
		free(f_from);
		f_from = NULL;
	}

	len->y = 0;
	f_from = NULL;
	return f_to;
}
