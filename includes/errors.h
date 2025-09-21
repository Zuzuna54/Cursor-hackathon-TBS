#ifndef _MORPHOSIS_ERRORS_H
# define _MORPHOSIS_ERRORS_H

# define MALLOC_FAIL_ERR 1
# define OPEN_FILE_ERR 2
# define ARGS_ERR 3
# define GRID_ERR 4
# define NO_ARG_ERR 5
# define BAD_FILE_ERR 6

# define MALLOC_FAIL "\nERROR: Could not allocate memory\n"
# define OPEN_FILE "\nERROR: Could not open the file\n"
# define GRID "\nERROR: Invalid step size\n min: 0.00001 | max: 0.5\n"
# define SMALL_S_SIZE "\nWARNING: Small step size —-- model display may lag considerably\nEnter 0 to proceed    |     1 to enter new value       |        2 to exit: "
# define ASK_SIZE "Please enter step size: "
# define ASK_ITER "Please enter number of iterations: "

# define ARGS "\nERROR: Invalid program arguments\n"
# define USAGE "\nUSAGE: \n./morphosis *step_size* *q.x* *q.y* *q.z* *q.w*\n./morphosis -d\t\t\t\t\t\t| to use default values\n./morphosis -m *file_name.mat*\t\t\t\t| to read data from matrix\n./morphosis -p *file_name*\t\t\t\t| to read data from poem\n\n"
# define NO_ARG "\nThis program calculates, displays and saves a 4d Julia set as an OBJ file in the current directory\nWhen fractal is displayed, press ESC to exit or S to save and export the mesh\n"

# define BAD_FILE "\nERROR: Invalid data in the file\n\n"

#endif
