cmake_minimum_required(VERSION 3.17)
project(morphosis)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

include_directories(includes)
include_directories(shaders)
include_directories(libft)
include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl@1.1/include)
link_directories(/usr/local/lib)

find_library(GLFW_LIB glfw HINTS /usr/local/lib)
find_library(GLEW_LIB GLEW HINTS /usr/local/lib)
find_library(SSL_LIB ssl HINTS /usr/local/opt/openssl@1.1/lib)
find_library(CRYPTO_LIB crypto HINTS /usr/local/opt/openssl@1.1/lib)

add_executable(morphosis
        libft/get_next_line.h
        libft/libft.h

        shaders/vertex.shader
        shaders/fragment.shader

        includes/morphosis.h
        includes/gl_includes.h
        includes/stb_image.h
        includes/errors.h
        includes/lib_complex.h
        includes/structures.h
        includes/look-up.h
        includes/obj.h
        includes/matrix.h

        srcs/main.c
        srcs/init.c
        srcs/cleanup.c
        srcs/errors.c
        srcs/utils.c
        srcs/point_cloud.c
        srcs/build_fractal.c
        srcs/sample_julia.c
        srcs/polygonisation.c
        srcs/write_obj.c

        srcs/lib_complex.c

        srcs/gl_draw.c
        srcs/gl_utils.c
        srcs/gl_buffers.c
        srcs/gl_build.c
        srcs/gl_points.c
        srcs/gl_init.c
        srcs/gl_calculations.c

        srcs/obj.c

        srcs/matrix_converter.c
        srcs/matrix_hash.c
        srcs/matrix_generate_coordinates.c
        srcs/matrix_read.c
        srcs/poem.c
        )

target_link_libraries(morphosis 
    ${GLFW_LIB} 
    ${GLEW_LIB} 
    ${SSL_LIB} 
    ${CRYPTO_LIB}
    "-framework OpenGL"
)