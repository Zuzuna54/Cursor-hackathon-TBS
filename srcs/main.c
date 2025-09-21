#include "morphosis.h"

static t_data 						*get_args(int argv, char **argc)
{
	t_data					*data;
	float 					s_size;
	int						iter;
	float4 					q;
	t_mat_conv_data			mat;

	data = NULL;
	if (argv == 1)
		error(NO_ARG_ERR, NULL);
	if (argv != 6)
	{
        if (argv == 2 && !(strcmp(argc[1], "-d")))
		{
			data = init_data();
			return data;
		}
        else if (argv == 3 && !(strcmp(argc[1], "-m")))
		{
			process_matrix(argc[2], &mat, MATRIX);
			data = init_data();
			data->fract->step_size = mat.step_size;
			data->fract->julia->c = mat.q;
			data->fract->julia->max_iter = mat.iter;
			return data;
		}
        else if (argv == 3 && !(strcmp(argc[1], "-p")))
		{
			process_matrix(argc[2], &mat, POEM);
			data = init_data();
			data->fract->step_size = mat.step_size;
			data->fract->julia->c = mat.q;
			data->fract->julia->max_iter = mat.iter;
			return data;
		}
		error(ARGS_ERR, NULL);
	}
	if ((s_size = (float)strtod(argc[1], NULL)) < 0.00001 || s_size > 1)
		s_size = s_size_warning(s_size);

	printf(ASK_ITER);
	fscanf(stdin, "%d", &iter);

	q.x = (float)strtod(argc[2], NULL);
	q.y = (float)strtod(argc[3], NULL);
	q.z = (float)strtod(argc[4], NULL);
	q.w = (float)strtod(argc[5], NULL);
	data = init_data();

	data->fract->step_size = s_size;
	data->fract->julia->c = q;
	data->fract->julia->max_iter = iter;
	return data;
}

int 						main(int argv, char **argc)
{
	t_data 					*data;

	data = get_args(argv, argc);
	calculate_point_cloud(data);
	gl_retrieve_tris(data);
	clean_calcs(data);

	run_graphics_enhanced(data, data->fract->p1, data->fract->p0);
	if (data->gl->export)
	{
		printf("\nEXPORTING----\n");
		export_obj(data);
		printf("DONE\n");
	}

	clean_up(data);
	return 0;
}
