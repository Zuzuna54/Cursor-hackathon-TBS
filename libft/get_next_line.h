#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef LIBFT_H
#  include "libft.h"
# endif
# include <limits.h>

# define BUFF_SIZE 5

int					get_next_line(int const fd, char **line);

#endif
