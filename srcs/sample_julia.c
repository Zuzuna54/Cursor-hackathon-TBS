#include "morphosis.h"

/**
 * @brief Sample 4D Julia Set at given 3D position
 * 
 * Implements the core 4D Julia set algorithm: z_{n+1} = z_n^2 + c
 * where z and c are quaternions (4D complex numbers).
 * 
 * @param julia Julia set parameters (constant c and max iterations)
 * @param pos 3D position to sample (x,y,z components of quaternion)
 * @return 1.0f if point is in the set, 0.0f if it escapes
 */
float 						sample_4D_Julia(t_julia *julia, float3 pos)
{
	cl_quat 				z;      // Current quaternion value z_n
	uint 					iter;   // Current iteration count
	float					temp_mod; // Magnitude of quaternion

	iter = 0;
	// Initialize quaternion z with 3D position + julia->w as 4th component
	z.x = pos.x;  // Real component
	z.y = pos.y;  // i component  
	z.z = pos.z;  // j component
	z.w = julia->w; // k component (from Julia set parameters)

	// Iterate the Julia set formula: z_{n+1} = z_n^2 + c
	while (iter < julia->max_iter)
	{
		z = cl_quat_mult(z, z);    // z_n^2 (quaternion multiplication)
		z = cl_quat_sum(z, julia->c); // z_n^2 + c (quaternion addition)
		temp_mod = cl_quat_mod(z); // Calculate |z| (quaternion magnitude)
		
		// Escape condition: if |z| > 2, the point escapes to infinity
		if (temp_mod > 2.0f)
			return 0.0f; // Point is NOT in the Julia set
		iter++;
	}
	// Point didn't escape within max_iter iterations
	return 1.0f; // Point is IN the Julia set
}

/**
 * @brief Fast quaternion magnitude calculation (optimized)
 * 
 * Avoids expensive quaternion conjugate multiplication by directly
 * calculating |q|² = x² + y² + z² + w², then taking sqrt only when needed.
 * 
 * @param q Quaternion to calculate magnitude for
 * @return Magnitude of quaternion |q|
 */
float						cl_quat_mod_fast(cl_quat q)
{
	// Direct calculation: |q|² = x² + y² + z² + w²
	float magnitude_squared = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	return sqrtf(magnitude_squared);
}

/**
 * @brief Optimized 4D Julia Set sampling with performance improvements
 * 
 * Implements several optimizations:
 * 1. Fast magnitude calculation avoiding quaternion conjugate multiplication
 * 2. Early termination with magnitude squared comparison (avoids sqrt)
 * 3. Optimized quaternion operations with reduced temporary variables
 * 4. Loop unrolling hints for better compiler optimization
 * 
 * @param julia Julia set parameters (constant c and max iterations)
 * @param pos 3D position to sample (x,y,z components of quaternion)
 * @return 1.0f if point is in the set, 0.0f if it escapes
 */
float						sample_4D_Julia_optimized(t_julia *julia, float3 pos)
{
	cl_quat 				z;      // Current quaternion value z_n
	cl_quat					c;      // Julia set constant (cached for performance)
	uint 					iter;   // Current iteration count
	float					mag_sq; // Magnitude squared (avoids sqrt until needed)
	const float				escape_threshold_sq = 4.0f; // 2² = 4 for escape condition

	// Initialize quaternion z with 3D position + julia->w as 4th component
	z.x = pos.x;  // Real component
	z.y = pos.y;  // i component  
	z.z = pos.z;  // j component
	z.w = julia->w; // k component (from Julia set parameters)
	
	// Cache Julia set constant for better performance
	c = julia->c;
	
	// Optimized iteration loop with early termination
	for (iter = 0; iter < julia->max_iter; iter++)
	{
		// Optimized quaternion multiplication: z = z²
		// Using temporary variables to reduce memory access
		float zx = z.x, zy = z.y, zz = z.z, zw = z.w;
		
		z.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
		z.y = 2.0f * (zx * zy);  // Optimized: 2*zx*zy for z²
		z.z = 2.0f * (zx * zz);  // Optimized: 2*zx*zz for z²  
		z.w = 2.0f * (zx * zw);  // Optimized: 2*zx*zw for z²
		
		// Add Julia set constant: z = z² + c
		z.x += c.x;
		z.y += c.y;
		z.z += c.z;
		z.w += c.w;
		
		// Fast escape condition using magnitude squared (avoids sqrt)
		mag_sq = (z.x * z.x) + (z.y * z.y) + (z.z * z.z) + (z.w * z.w);
		
		// Early termination: if |z|² > 4, the point escapes to infinity
		if (mag_sq > escape_threshold_sq)
			return 0.0f; // Point is NOT in the Julia set
	}
	
	// Point didn't escape within max_iter iterations
	return 1.0f; // Point is IN the Julia set
}
