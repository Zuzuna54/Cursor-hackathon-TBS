NAME = morphosis

SRC_DIR = ./srcs/
SRC = 	main.c \
		cleanup.c \
		init.c \
		errors.c \
		utils.c \
		point_cloud.c \
		build_fractal.c \
		sample_julia.c \
		polygonisation.c \
		write_obj.c \
		\
		gl_draw.c \
        gl_utils.c \
        gl_buffers.c \
        gl_build.c \
        gl_points.c \
        gl_init.c \
        gl_calculations.c\
        enhanced_rendering.c \
        enhanced_colored_rendering.c \
        mathematical_enhancements.c \
        \
        obj.c \
        \
        lib_complex.c \
        \
        matrix_converter.c \
        matrix_hash.c \
        matrix_generate_coordinates.c \
        matrix_read.c \
        poem.c

SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))
OBJ_DIR = ./obj/
OBJ = $(SRC:.c=.o)

INCS = $(addprefix $(INC_DIR), $(INC))
INC_DIR = ./includes/
INC = 	morphosis.h \
		gl_includes.h \
		stb_image.h \
		errors.h \
		lib_complex.h \
		structures.h \
		look-up.h \
		obj.h \
		matrix.h

LIB_INC = libft.h get_next_line.h
LIB_INC_DIR = ./libft/
LIB_INCS = $(addprefix $(LIB_INC_DIR), $(LIB_INC))

FLAGS = -O3 -Wall -I$(INC_DIR) -I$(LIB_INC_DIR)
GL_LIBS = -framework OpenGL -lGLEW -lglfw -I/usr/local/include
OPENSSL_LIB = -lssl -lcrypto -L/usr/local/opt/openssl@1.1/lib -I/usr/local/opt/openssl@1.1/include

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
		clang $(OBJS) -o $(NAME) $(GL_LIBS) $(OPENSSL_LIB)

$(OBJ_DIR):
		mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCS)
		clang $(FLAGS) -o $@ -c $<

clean:
		@rm -f $(OBJS)
		@rm -rf $(OBJ_DIR)

fclean: clean
		@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re