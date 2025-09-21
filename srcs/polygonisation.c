#include "morphosis.h"
#include "look-up.h"

static uint 				getCubeIndex(float *v_val, uint pos)
{
	uint					cubeindex;

	cubeindex = 0;
	if (v_val[pos + 0])
		cubeindex |= 1;
	if (v_val[pos + 1])
		cubeindex |= 2;
	if (v_val[pos + 2])
		cubeindex |= 4;
	if (v_val[pos + 3])
		cubeindex |= 8;
	if (v_val[pos + 4])
		cubeindex |= 16;
	if (v_val[pos + 5])
		cubeindex |= 32;
	if (v_val[pos + 6])
		cubeindex |= 64;
	if (v_val[pos + 7])
		cubeindex |= 128;
	return cubeindex;
}

static float3				interpolate(float3 p0, float3 p1, float v0, float v1)
{
	float					mu;
	float3					p;

	if (v0 == 1.0f)
		return p0;
	if (v1 == 1.0f)
		return p1;
	if ((v1 - v0) == 0.0f)
		return p0;
	mu = (1.0f - v0) / (v1 - v0);
	p.x = p0.x + mu * (p1.x - p0.x);
	p.y = p0.y + mu * (p1.y - p0.y);
	p.z = p0.z + mu * (p1.z - p0.z);
	return p;
}

static float3				*get_vertices(uint cubeindex, float3 *v_pos, float *v_val, uint pos)
{
	float3					*vertlist;

	if (!(vertlist = (float3 *)malloc(12 * sizeof(float3))))
		return NULL;

	if (edgetable[cubeindex] & 1)
		vertlist[0] = interpolate(v_pos[pos + 0], v_pos[pos + 1], v_val[pos + 0], v_val[pos + 1]);
	if (edgetable[cubeindex] & 2)
		vertlist[1] = interpolate(v_pos[pos + 1], v_pos[pos + 2], v_val[pos + 1], v_val[pos + 2]);
	if (edgetable[cubeindex] & 4)
		vertlist[2] = interpolate(v_pos[pos + 2], v_pos[pos + 3], v_val[pos + 2], v_val[pos + 3]);
	if (edgetable[cubeindex] & 8)
		vertlist[3] = interpolate(v_pos[pos + 3], v_pos[pos + 0], v_val[pos + 3], v_val[pos + 0]);
	if (edgetable[cubeindex] & 16)
		vertlist[4] = interpolate(v_pos[pos + 4], v_pos[pos + 5], v_val[pos + 4], v_val[pos + 5]);
	if (edgetable[cubeindex] & 32)
		vertlist[5] = interpolate(v_pos[pos + 5], v_pos[pos + 6], v_val[pos + 5], v_val[pos + 6]);
	if (edgetable[cubeindex] & 64)
		vertlist[6] = interpolate(v_pos[pos + 6], v_pos[pos + 7], v_val[pos + 6], v_val[pos + 7]);
	if (edgetable[cubeindex] & 128)
		vertlist[7] = interpolate(v_pos[pos + 7], v_pos[pos + 4], v_val[pos + 7], v_val[pos + 4]);
	if (edgetable[cubeindex] & 256)
		vertlist[8] = interpolate(v_pos[pos + 0], v_pos[pos + 4], v_val[pos + 0], v_val[pos + 4]);
	if (edgetable[cubeindex] & 512)
		vertlist[9] = interpolate(v_pos[pos + 1], v_pos[pos + 5], v_val[pos + 1], v_val[pos + 5]);
	if (edgetable[cubeindex] & 1024)
		vertlist[10] = interpolate(v_pos[pos + 2], v_pos[pos + 6], v_val[pos + 2], v_val[pos + 6]);
	if (edgetable[cubeindex] & 2048)
		vertlist[11] = interpolate(v_pos[pos + 3], v_pos[pos + 7], v_val[pos + 3], v_val[pos + 7]);
	return vertlist;
}

static float3						**package_triangles(float3 *vertlist, uint cubeindex, uint i, t_data *data)
{
	uint2 					len;
	float3 					**tris_new;

	len.x = 0;
	len.y = 1;
	tris_new = NULL;
	if (!(tris_new = alloc_float3_arr(tris_new, &len)))
		error(MALLOC_FAIL_ERR, data);

	tris_new[0][0] = vertlist[tritable[cubeindex][i]];
	tris_new[0][1] = vertlist[tritable[cubeindex][i + 1]];
	tris_new[0][2] = vertlist[tritable[cubeindex][i + 2]];
	return tris_new;
}

/**
 * @brief Marching Cubes Algorithm - Extract 3D surface from scalar field
 * 
 * Implements the classic Marching Cubes algorithm to generate triangular mesh
 * from the 4D Julia set scalar field. Each cube in the 3D grid is analyzed
 * to determine surface intersection and generate appropriate triangles.
 * 
 * @param v_pos Array of 8 vertex positions for current cube
 * @param v_val Array of 8 scalar values (0.0 or 1.0 from Julia set)
 * @param pos Current position in the grid
 * @param data Main data structure for error handling
 * @return Array of triangles representing the surface, or NULL if no surface
 */
float3 						**polygonise(float3 *v_pos, float *v_val, uint2 *pos, t_data *data)
{
	float3					**tris;      // Final triangle array
	float3 					**tris_new;  // Temporary triangle array
	float3					*vertlist;   // Interpolated edge vertices
	uint 					cubeindex;   // Cube configuration index (0-255)
	uint 					i;           // Triangle table iterator
	uint2 					len;         // Array length tracking

	tris = NULL;
	tris_new = NULL;
	vertlist = NULL;
	i = 0;
	len.x = 0;
	
	// Step 1: Determine cube configuration from 8 vertex values
	cubeindex = getCubeIndex(v_val, pos->x);
	
	// Step 2: Check if surface intersects this cube (edgetable lookup)
	if (edgetable[cubeindex] == 0)
		return NULL; // No surface intersection, skip this cube
	
	// Step 3: Calculate interpolated vertices on cube edges where surface crosses
	if (!(vertlist = get_vertices(cubeindex, v_pos, v_val, pos->x)))
		error(MALLOC_FAIL_ERR, data);

	// Step 4: Generate triangles using triangle table lookup
	while ((int)tritable[cubeindex][i] != -1) // -1 terminates triangle list
	{
		// Create triangle from 3 consecutive vertex indices in triangle table
		tris_new = package_triangles(vertlist, cubeindex, i, data);
		len.y = 1;
		
		// Concatenate new triangle to existing triangle array
		if (!(tris = arr_float3_cat(tris_new, tris, &len)))
			error(MALLOC_FAIL_ERR, data);
		i += 3; // Move to next triangle (3 vertices per triangle)
	}
	data->len.y = len.x;

	free(vertlist); // Clean up temporary vertex list
	return tris; // Return generated triangles
}

/**
 * @brief Optimized Marching Cubes Algorithm with memory pool
 * 
 * Enhanced version that uses pre-allocated vertex list to eliminate
 * malloc/free overhead in the critical marching cubes loop.
 * 
 * @param v_pos Array of 8 vertex positions for current cube
 * @param v_val Array of 8 scalar values (0.0 or 1.0 from Julia set)
 * @param pos Current position in the grid
 * @param data Main data structure with pre-allocated vertex list
 * @return Array of triangles representing the surface, or NULL if no surface
 */
float3 						**polygonise_optimized(float3 *v_pos, float *v_val, uint2 *pos, t_data *data)
{
	float3					**tris;      // Final triangle array
	float3 					**tris_new;  // Temporary triangle array
	float3					*vertlist;   // Pre-allocated edge vertices (optimization!)
	uint 					cubeindex;   // Cube configuration index (0-255)
	uint 					i;           // Triangle table iterator
	uint2 					len;         // Array length tracking

	tris = NULL;
	tris_new = NULL;
	i = 0;
	len.x = 0;
	
	// Step 1: Determine cube configuration from 8 vertex values
	cubeindex = getCubeIndex(v_val, pos->x);
	
	// Step 2: Check if surface intersects this cube (edgetable lookup)
	if (edgetable[cubeindex] == 0)
		return NULL; // No surface intersection, skip this cube
	
	// Step 3: Use pre-allocated vertex list (OPTIMIZATION: no malloc!)
	vertlist = data->mc_vertlist;
	
	// Calculate interpolated vertices on cube edges where surface crosses
	// Optimized: only calculate vertices that are actually used
	if (edgetable[cubeindex] & 1)
		vertlist[0] = interpolate(v_pos[pos->x + 0], v_pos[pos->x + 1], v_val[pos->x + 0], v_val[pos->x + 1]);
	if (edgetable[cubeindex] & 2)
		vertlist[1] = interpolate(v_pos[pos->x + 1], v_pos[pos->x + 2], v_val[pos->x + 1], v_val[pos->x + 2]);
	if (edgetable[cubeindex] & 4)
		vertlist[2] = interpolate(v_pos[pos->x + 2], v_pos[pos->x + 3], v_val[pos->x + 2], v_val[pos->x + 3]);
	if (edgetable[cubeindex] & 8)
		vertlist[3] = interpolate(v_pos[pos->x + 3], v_pos[pos->x + 0], v_val[pos->x + 3], v_val[pos->x + 0]);
	if (edgetable[cubeindex] & 16)
		vertlist[4] = interpolate(v_pos[pos->x + 4], v_pos[pos->x + 5], v_val[pos->x + 4], v_val[pos->x + 5]);
	if (edgetable[cubeindex] & 32)
		vertlist[5] = interpolate(v_pos[pos->x + 5], v_pos[pos->x + 6], v_val[pos->x + 5], v_val[pos->x + 6]);
	if (edgetable[cubeindex] & 64)
		vertlist[6] = interpolate(v_pos[pos->x + 6], v_pos[pos->x + 7], v_val[pos->x + 6], v_val[pos->x + 7]);
	if (edgetable[cubeindex] & 128)
		vertlist[7] = interpolate(v_pos[pos->x + 7], v_pos[pos->x + 4], v_val[pos->x + 7], v_val[pos->x + 4]);
	if (edgetable[cubeindex] & 256)
		vertlist[8] = interpolate(v_pos[pos->x + 0], v_pos[pos->x + 4], v_val[pos->x + 0], v_val[pos->x + 4]);
	if (edgetable[cubeindex] & 512)
		vertlist[9] = interpolate(v_pos[pos->x + 1], v_pos[pos->x + 5], v_val[pos->x + 1], v_val[pos->x + 5]);
	if (edgetable[cubeindex] & 1024)
		vertlist[10] = interpolate(v_pos[pos->x + 2], v_pos[pos->x + 6], v_val[pos->x + 2], v_val[pos->x + 6]);
	if (edgetable[cubeindex] & 2048)
		vertlist[11] = interpolate(v_pos[pos->x + 3], v_pos[pos->x + 7], v_val[pos->x + 3], v_val[pos->x + 7]);

	// Step 4: Generate triangles using triangle table lookup
	while ((int)tritable[cubeindex][i] != -1) // -1 terminates triangle list
	{
		// Create triangle from 3 consecutive vertex indices in triangle table
		tris_new = package_triangles(vertlist, cubeindex, i, data);
		len.y = 1;
		
		// Concatenate new triangle to existing triangle array
		if (!(tris = arr_float3_cat(tris_new, tris, &len)))
			error(MALLOC_FAIL_ERR, data);
		i += 3; // Move to next triangle (3 vertices per triangle)
	}
	data->len.y = len.x;

	// No need to free vertlist - it's pre-allocated and reused!
	return tris; // Return generated triangles
}
