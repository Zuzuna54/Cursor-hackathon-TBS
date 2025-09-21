#include "morphosis.h"

static void 					ft_itoa(int num, char *str)
{
	if (!str)
		return;
	sprintf(str, "%d", num);
}

static char						*matrix_to_string(int **matrix)
{
	char 						*res;
	char						tmp[4];

	if (!(res = (char *)malloc(STR_BUFFER * sizeof(char))))
		error(MALLOC_FAIL_ERR, NULL);
	bzero(res, STR_BUFFER);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			bzero(tmp, 4);
			ft_itoa(matrix[i][j], tmp);
			res = strcat(res, tmp);
		}
	}
	return res;
}

static void 					print_res(t_mat_conv_data *data)
{
	printf("x: %f\n", data->q.x);
	printf("y: %f\n", data->q.y);
	printf("z: %f\n", data->q.z);
	printf("w: %f\n", data->q.w);
}

void 							matrix_hash(int **matrix, t_mat_conv_data *data)
{
	char 						*mat_string;
	unsigned char 				*hash;

	mat_string = matrix_to_string(matrix);
	hash = SHA256((const unsigned char *)mat_string, strlen(mat_string), 0);
	free(mat_string);
	get_coords_from_hash(hash, data);
	print_res(data);
	printf(ASK_SIZE);
	fscanf(stdin, "%f", &data->step_size);
	if (data->step_size < 0.00001 || data->step_size > 1)
		data->step_size = s_size_warning(data->step_size);
	printf(ASK_ITER);
	fscanf(stdin, "%d", &data->iter);
}

void 							matrix_hash2(char *matrix, t_mat_conv_data *data)
{
	unsigned char 				*hash;

	hash = SHA256((const unsigned char *)matrix, strlen(matrix), 0);
	free(matrix);
	get_coords_from_hash(hash, data);
	print_res(data);
	printf(ASK_SIZE);
	fscanf(stdin, "%f", &data->step_size);
	if (data->step_size < 0.00001 || data->step_size > 1)
		data->step_size = s_size_warning(data->step_size);
	printf(ASK_ITER);
	fscanf(stdin, "%d", &data->iter);
}
