#include "morphosis.h"

/**
 * @brief Double precision quaternion operations for deep zoom
 */

double cl_quat_mod_d(cl_quat_d q)
{
    return sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
}

cl_quat_d cl_quat_mult_d(cl_quat_d q1, cl_quat_d q2)
{
    cl_quat_d result;
    result.x = (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) - (q1.w * q2.w);
    result.y = (q1.x * q2.y) + (q1.y * q2.x) + (q1.z * q2.w) - (q1.w * q2.z);
    result.z = (q1.x * q2.z) - (q1.y * q2.w) + (q1.z * q2.x) + (q1.w * q2.y);
    result.w = (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x);
    return result;
}

cl_quat_d cl_quat_sum_d(cl_quat_d q1, cl_quat_d q2)
{
    cl_quat_d result;
    result.x = q1.x + q2.x;
    result.y = q1.y + q2.y;
    result.z = q1.z + q2.z;
    result.w = q1.w + q2.w;
    return result;
}

/**
 * @brief Deep zoom Julia set sampling with double precision
 * 
 * Uses double precision arithmetic for extreme zoom levels where
 * single precision floating point loses accuracy.
 */
float sample_4D_Julia_deep_zoom(t_julia *julia, double3 pos, double zoom_level)
{
    cl_quat_d z, c;
    uint iter;
    double mag_sq;
    const double escape_threshold_sq = 4.0;
    
    // Initialize with high precision
    z.x = pos.x / zoom_level;
    z.y = pos.y / zoom_level;
    z.z = pos.z / zoom_level;
    z.w = (double)julia->w / zoom_level;
    
    // Convert Julia constant to double precision
    c.x = (double)julia->c.x;
    c.y = (double)julia->c.y;
    c.z = (double)julia->c.z;
    c.w = (double)julia->c.w;
    
    for (iter = 0; iter < julia->max_iter; iter++)
    {
        // Double precision quaternion squaring
        double zx = z.x, zy = z.y, zz = z.z, zw = z.w;
        
        z.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
        z.y = 2.0 * (zx * zy);
        z.z = 2.0 * (zx * zz);
        z.w = 2.0 * (zx * zw);
        
        // Add constant
        z.x += c.x;
        z.y += c.y;
        z.z += c.z;
        z.w += c.w;
        
        // Check escape condition
        mag_sq = (z.x * z.x) + (z.y * z.y) + (z.z * z.z) + (z.w * z.w);
        if (mag_sq > escape_threshold_sq)
            return 0.0f;
    }
    
    return 1.0f;
}

/**
 * @brief Mandelbrot set sampling in 4D
 * 
 * Implements 4D Mandelbrot set: z_{n+1} = z_n^2 + c
 * where c is the current position and z starts at origin.
 */
float sample_4D_Mandelbrot(t_julia *julia, float3 pos)
{
    cl_quat z, c;
    uint iter;
    float mag_sq;
    const float escape_threshold_sq = 4.0f;
    
    // For Mandelbrot, c is the position and z starts at origin
    c.x = pos.x;
    c.y = pos.y;
    c.z = pos.z;
    c.w = julia->w; // Use w parameter from Julia settings
    
    // Start z at origin (modified by Julia constant for variation)
    z.x = julia->c.x * 0.1f; // Small offset for interesting variations
    z.y = julia->c.y * 0.1f;
    z.z = julia->c.z * 0.1f;
    z.w = julia->c.w * 0.1f;
    
    for (iter = 0; iter < julia->max_iter; iter++)
    {
        // Quaternion squaring
        float zx = z.x, zy = z.y, zz = z.z, zw = z.w;
        
        z.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
        z.y = 2.0f * (zx * zy);
        z.z = 2.0f * (zx * zz);
        z.w = 2.0f * (zx * zw);
        
        // Add position (c)
        z.x += c.x;
        z.y += c.y;
        z.z += c.z;
        z.w += c.w;
        
        // Check escape condition
        mag_sq = (z.x * z.x) + (z.y * z.y) + (z.z * z.z) + (z.w * z.w);
        if (mag_sq > escape_threshold_sq)
            return 0.0f;
    }
    
    return 1.0f;
}

/**
 * @brief Alternative quaternion formulas for variety
 * 
 * Implements different iteration formulas:
 * 0: Standard z^2 + c
 * 1: z^3 + c (cubic)
 * 2: z^2 + z + c (quadratic with linear term)
 * 3: |z|^2 - z^2 + c (magnitude-based)
 */
float sample_4D_Julia_alternative_formula(t_julia *julia, float3 pos, int formula)
{
    cl_quat z, c;
    uint iter;
    float mag_sq;
    const float escape_threshold_sq = 16.0f; // Higher threshold for some formulas
    
    // Initialize
    z.x = pos.x;
    z.y = pos.y;
    z.z = pos.z;
    z.w = julia->w;
    
    c.x = julia->c.x;
    c.y = julia->c.y;
    c.z = julia->c.z;
    c.w = julia->c.w;
    
    for (iter = 0; iter < julia->max_iter; iter++)
    {
        float zx = z.x, zy = z.y, zz = z.z, zw = z.w;
        cl_quat z_new;
        
        switch (formula)
        {
            case 0: // Standard z^2 + c
                z_new.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
                z_new.y = 2.0f * (zx * zy);
                z_new.z = 2.0f * (zx * zz);
                z_new.w = 2.0f * (zx * zw);
                break;
                
            case 1: // z^3 + c (cubic)
                {
                    // First compute z^2
                    cl_quat z2;
                    z2.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
                    z2.y = 2.0f * (zx * zy);
                    z2.z = 2.0f * (zx * zz);
                    z2.w = 2.0f * (zx * zw);
                    
                    // Then z^3 = z * z^2
                    z_new.x = (zx * z2.x) - (zy * z2.y) - (zz * z2.z) - (zw * z2.w);
                    z_new.y = (zx * z2.y) + (zy * z2.x) + (zz * z2.w) - (zw * z2.z);
                    z_new.z = (zx * z2.z) - (zy * z2.w) + (zz * z2.x) + (zw * z2.y);
                    z_new.w = (zx * z2.w) + (zy * z2.z) - (zz * z2.y) + (zw * z2.x);
                }
                break;
                
            case 2: // z^2 + z + c
                z_new.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw) + zx;
                z_new.y = 2.0f * (zx * zy) + zy;
                z_new.z = 2.0f * (zx * zz) + zz;
                z_new.w = 2.0f * (zx * zw) + zw;
                break;
                
            case 3: // |z|^2 - z^2 + c (magnitude-based)
                {
                    float mag_sq_z = (zx * zx) + (zy * zy) + (zz * zz) + (zw * zw);
                    z_new.x = mag_sq_z - ((zx * zx) - (zy * zy) - (zz * zz) - (zw * zw));
                    z_new.y = -2.0f * (zx * zy);
                    z_new.z = -2.0f * (zx * zz);
                    z_new.w = -2.0f * (zx * zw);
                }
                break;
                
            default:
                // Fallback to standard formula
                z_new.x = (zx * zx) - (zy * zy) - (zz * zz) - (zw * zw);
                z_new.y = 2.0f * (zx * zy);
                z_new.z = 2.0f * (zx * zz);
                z_new.w = 2.0f * (zx * zw);
                break;
        }
        
        // Add constant
        z.x = z_new.x + c.x;
        z.y = z_new.y + c.y;
        z.z = z_new.z + c.z;
        z.w = z_new.w + c.w;
        
        // Check escape condition
        mag_sq = (z.x * z.x) + (z.y * z.y) + (z.z * z.z) + (z.w * z.w);
        if (mag_sq > escape_threshold_sq)
            return 0.0f;
    }
    
    return 1.0f;
}

/**
 * @brief Adaptive grid refinement for areas with high detail
 * 
 * Analyzes local complexity and subdivides grid cells that show
 * high variation in fractal values.
 */
int should_refine_grid_cell(t_data *data, float3 center, float cell_size, int current_depth)
{
    if (current_depth >= data->max_grid_depth)
        return 0;
    
    // Sample corners and center of cell
    float samples[9];
    float3 offsets[9] = {
        {-cell_size/2, -cell_size/2, -cell_size/2}, // corners
        { cell_size/2, -cell_size/2, -cell_size/2},
        {-cell_size/2,  cell_size/2, -cell_size/2},
        { cell_size/2,  cell_size/2, -cell_size/2},
        {-cell_size/2, -cell_size/2,  cell_size/2},
        { cell_size/2, -cell_size/2,  cell_size/2},
        {-cell_size/2,  cell_size/2,  cell_size/2},
        { cell_size/2,  cell_size/2,  cell_size/2},
        { 0.0f,         0.0f,         0.0f}         // center
    };
    
    // Sample fractal at each point
    for (int i = 0; i < 9; i++)
    {
        float3 sample_pos = {
            center.x + offsets[i].x,
            center.y + offsets[i].y,
            center.z + offsets[i].z
        };
        
        switch (data->fractal_type)
        {
            case 0: // Julia set
                if (data->use_double_precision && data->zoom_level > 1000.0)
                {
                    double3 pos_d = {sample_pos.x, sample_pos.y, sample_pos.z};
                    samples[i] = sample_4D_Julia_deep_zoom(data->fract->julia, pos_d, data->zoom_level);
                }
                else if (data->quaternion_formula != 0)
                {
                    samples[i] = sample_4D_Julia_alternative_formula(data->fract->julia, sample_pos, data->quaternion_formula);
                }
                else
                {
                    samples[i] = sample_4D_Julia_optimized(data->fract->julia, sample_pos);
                }
                break;
                
            case 1: // Mandelbrot set
                samples[i] = sample_4D_Mandelbrot(data->fract->julia, sample_pos);
                break;
                
            default:
                samples[i] = sample_4D_Julia_optimized(data->fract->julia, sample_pos);
                break;
        }
    }
    
    // Calculate variation (standard deviation)
    float mean = 0.0f;
    for (int i = 0; i < 9; i++)
        mean += samples[i];
    mean /= 9.0f;
    
    float variance = 0.0f;
    for (int i = 0; i < 9; i++)
    {
        float diff = samples[i] - mean;
        variance += diff * diff;
    }
    variance /= 9.0f;
    
    float std_dev = sqrtf(variance);
    
    // Refine if variation exceeds threshold
    return std_dev > data->detail_threshold;
}

/**
 * @brief Supersampling for anti-aliasing
 * 
 * Takes multiple samples per grid point and averages them
 * to reduce aliasing artifacts.
 */
float sample_with_supersampling(t_data *data, float3 pos)
{
    if (data->supersampling <= 1)
    {
        // No supersampling - temporarily disable supersampling and call main function
        int original_supersampling = data->supersampling;
        data->supersampling = 1;
        float result = sample_fractal_enhanced(data, pos);
        data->supersampling = original_supersampling;
        return result;
    }
    
    // Supersampling enabled
    float total = 0.0f;
    int samples = data->supersampling;
    float offset = data->fract->step_size / (float)(samples * 2);
    
    for (int x = 0; x < samples; x++)
    {
        for (int y = 0; y < samples; y++)
        {
            for (int z = 0; z < samples; z++)
            {
                float3 sample_pos = {
                    pos.x + (x - samples/2) * offset,
                    pos.y + (y - samples/2) * offset,
                    pos.z + (z - samples/2) * offset
                };
                
                // Temporarily disable supersampling and call main function
                int original_supersampling = data->supersampling;
                data->supersampling = 1;
                float sample_value = sample_fractal_enhanced(data, sample_pos);
                data->supersampling = original_supersampling;
                
                total += sample_value;
            }
        }
    }
    
    return total / (float)(samples * samples * samples);
}

/**
 * @brief Enhanced fractal sampling with all mathematical improvements
 * 
 * Central function that applies all enhancements:
 * - Deep zoom with double precision
 * - Alternative fractal types
 * - Different quaternion formulas
 * - Supersampling anti-aliasing
 * - Adaptive sampling
 */
float sample_fractal_enhanced(t_data *data, float3 pos)
{
    // Use supersampling if enabled
    if (data->supersampling > 1)
    {
        return sample_with_supersampling(data, pos);
    }
    
    // Apply zoom level to position coordinates for all sampling methods
    float3 zoomed_pos = pos;
    if (data->zoom_level > 1.0)
    {
        zoomed_pos.x = pos.x / (float)data->zoom_level;
        zoomed_pos.y = pos.y / (float)data->zoom_level;
        zoomed_pos.z = pos.z / (float)data->zoom_level;
    }
    
    // Direct sampling based on fractal type and precision
    switch (data->fractal_type)
    {
        case 0: // Julia set
            if (data->use_double_precision && data->zoom_level > 1000.0)
            {
                double3 pos_d = {zoomed_pos.x, zoomed_pos.y, zoomed_pos.z};
                return sample_4D_Julia_deep_zoom(data->fract->julia, pos_d, data->zoom_level);
            }
            else if (data->quaternion_formula != 0)
            {
                return sample_4D_Julia_alternative_formula(data->fract->julia, zoomed_pos, data->quaternion_formula);
            }
            else
            {
                return sample_4D_Julia_optimized(data->fract->julia, zoomed_pos);
            }
            
        case 1: // Mandelbrot set
            return sample_4D_Mandelbrot(data->fract->julia, zoomed_pos);
            
        case 2: // Hybrid (future enhancement)
            // Combine Julia and Mandelbrot based on position
            {
                float julia_val = sample_4D_Julia_optimized(data->fract->julia, zoomed_pos);
                float mandel_val = sample_4D_Mandelbrot(data->fract->julia, zoomed_pos);
                float blend = 0.5f + 0.5f * sinf(zoomed_pos.x + zoomed_pos.y + zoomed_pos.z);
                return julia_val * blend + mandel_val * (1.0f - blend);
            }
            
        default:
            return sample_4D_Julia_optimized(data->fract->julia, zoomed_pos);
    }
}
