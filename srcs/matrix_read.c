#include <morphosis.h>
#include <libft.h>

#define SIZE 1296

static int						*alloc_mat_int(void)
{
	int							*res;

	if (!(res = (int *)malloc(SIZE * sizeof(int))))
		error(MALLOC_FAIL_ERR, NULL);
	return res;
}

static char						*alloc_mat_char(void)
{
	char						*res;

	if (!(res = (char *)malloc(SIZE * sizeof(char))))
		error(MALLOC_FAIL_ERR, NULL);
	bzero(res, SIZE);
	return res;
}

char							*read_matrix(FILE *stream)
{
	int 						*ints;
	char 						*res;
	int							c;

	ints = alloc_mat_int();
	c = 0;

	while (c < SIZE)
	{
		fscanf(stream, "%d1", &ints[c]);
		c++;
	}

	res = alloc_mat_char();
	c = 0;
	while (c < SIZE)
	{
		sprintf(&res[c], "%d", ints[c]);
		c++;
	}
	free(ints);

	return res;
}
