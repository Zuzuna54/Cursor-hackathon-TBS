#ifndef _MORPHOSIS_MATRIX_H
# define _MORPHOSIS_MATRIX_H

# include <openssl/sha.h>

# define STR_BUFFER 150
# define MATRIX 1
# define POEM 2

# define MODE 1
# define WHOLE 1
# define ZW 1

typedef struct 					s_mat_conv_data
{
	float4 						q;
	float 						step_size;
	int							iter;
}								t_mat_conv_data;

char							*read_matrix(FILE *stream);

void							process_matrix(char *file, t_mat_conv_data *data, int mode);
void							free_matrix1(int ***m);
int								***alloc_matrix1(void);
void							fill_matrix(int **matrix, int number, int reset);

void 							matrix_hash(int **matrix, t_mat_conv_data *data);
void 							matrix_hash2(char *matrix, t_mat_conv_data *data);

void 							get_coords_from_hash(unsigned char *hash, t_mat_conv_data *data);

int								***read_poem(FILE *stream);

#endif
