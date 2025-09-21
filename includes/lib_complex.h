#ifndef _LIB_COMPLEX_H
# define _LIB_COMPLEX_H

#include <math.h>

// Custom vector type definitions to replace OpenCL types
typedef struct {
    float x, y;
} float2;

typedef struct {
    float x, y, z;
} float3;

typedef struct {
    float x, y, z, w;
} float4;

typedef struct {
    unsigned int x, y;
} uint2;

// Standard type definitions
typedef unsigned int uint;

// Complex number and quaternion type definitions
typedef float2 cl_complex;
typedef float4 cl_quat;
typedef float  TYPE;

// Double precision types for deep zoom
typedef struct {
    double x, y;
} double2;

typedef struct {
    double x, y, z;
} double3;

typedef struct {
    double x, y, z, w;
} double4;

typedef double2 cl_complex_d;
typedef double4 cl_quat_d;

# define I_cl ((cl_complex){0.0f, 1.0f})
# define Pi_cl 3.14159265358979323846
# define E_cl 2.718281828459045235360

TYPE			cl_creal(cl_complex n);
TYPE			cl_cimag(cl_complex n);
TYPE			cl_cmod(cl_complex n);
cl_complex		cl_cadd(cl_complex a, cl_complex b);
cl_complex 		cl_cmult(cl_complex a, cl_complex b);
cl_complex 		cl_cpow(cl_complex base , int exp);
cl_complex 		cl_cdiv(cl_complex a, cl_complex b);
TYPE 			cl_carg(cl_complex a);
cl_complex 		cl_csqrt(cl_complex n);
cl_complex 		cl_cexp(cl_complex n);
cl_complex 		cl_clog(cl_complex z);
TYPE			cl_cdot(cl_complex a, cl_complex b);
cl_quat			cl_quat_mult(cl_quat q1, cl_quat q2);
cl_quat 		cl_quat_sum(cl_quat q1, cl_quat q2);
cl_quat 		cl_quat_conjugate(cl_quat q);
TYPE 			cl_quat_mod(cl_quat q);

#endif
