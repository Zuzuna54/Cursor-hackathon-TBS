#include "morphosis.h"

static float 				get_s_size(void)
{
	float					size;

	printf(ASK_SIZE);
	fscanf(stdin, "%f", &size);
	return size;
}

float						s_size_warning(float size)
{
	int						pass;
	int						exit_state;

	pass = 0;
	exit_state = 0;
	while (!pass)
	{
		if (size < 0.00001 || size > 0.5)
		{
			printf(GRID);
			size = get_s_size();
		}
		else if (size < 0.02)
		{
			printf(SMALL_S_SIZE);
			fscanf(stdin, "%d", &exit_state);
			if (exit_state == 1)
				size = get_s_size();
			if (exit_state == 2)
				exit(0);
			if (!exit_state)
				pass = 1;
		}
		else
			pass = 1;
	}
	return size;
}

void 						error(int errno, t_data *data)
{
	if (errno == MALLOC_FAIL_ERR)
		printf(MALLOC_FAIL);
	else if (errno == OPEN_FILE_ERR)
		printf(OPEN_FILE);
	else if (errno == ARGS_ERR)
		printf("%s%s", ARGS, USAGE);
	else if (errno == GRID_ERR)
		printf(GRID);
	else if (errno == NO_ARG_ERR)
		printf("%s%s", NO_ARG, USAGE);
	else if (errno == BAD_FILE_ERR)
		printf(BAD_FILE);
	clean_up(data);
	exit(1);
}
