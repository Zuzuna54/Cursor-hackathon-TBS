#include "morphosis.h"
#include <fcntl.h>
#include <unistd.h>

void 						makeShaderProgram(t_gl *gl)
{
	char 					*vertex_source;
	char 					*fragment_source;
	char 					**vertex_s;
	char 					**fragment_s;

	vertex_s = NULL;
	fragment_s = NULL;

	vertex_source = readShaderSource(VERTEX_SRC);
	vertex_s = &vertex_source;
	fragment_source = readShaderSource(FRAGMENT_SRC);
	fragment_s = &fragment_source;

	gl->vertexShader = createShader(GL_VERTEX_SHADER, vertex_s);
	gl->fragmentShader = createShader(GL_FRAGMENT_SHADER, fragment_s);

	createProgram(gl);
	vertex_s = NULL;
	fragment_s = NULL;
	free(vertex_source);
	free(fragment_source);
}

char						*readShaderSource(char *src_name)
{
	int						i;
	int						fd;
	char 					*src_string;

	if (!(src_string = (char *)malloc(SHADER_SRC_SIZE * sizeof(char))))
		error(MALLOC_FAIL_ERR, NULL);
	bzero(src_string, SHADER_SRC_SIZE);
	i = 0;
	fd = open(src_name, O_RDONLY);
	if (fd == 0 || fd == -1)
		error(OPEN_FILE_ERR, NULL);
	while (read(fd, &src_string[i], 1))
		i++;
	close(fd);
	return src_string;
}

GLuint 						createShader(GLenum type, char **src)
{
	char 				buffer[512];
	GLint 				status;
	GLuint 				shader;

	bzero(buffer, 512);
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar * const *)src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		if (type == GL_VERTEX_SHADER)
			printf("Vertex ");
		else if (type == GL_FRAGMENT_SHADER)
			printf("Fragment ");
		printf("shader comp failed:\n");
		printf("%s\n", buffer);
		exit(1);
	}
	return shader;
}

void 						createProgram(t_gl *gl)
{
	char 				buffer[512];
	GLint 				status;

	bzero(buffer, 512);
	gl->shaderProgram = glCreateProgram();
	if (gl->vertexShader)
		glAttachShader(gl->shaderProgram, gl->vertexShader);
	if (gl->fragmentShader)
		glAttachShader(gl->shaderProgram, gl->fragmentShader);
	glLinkProgram(gl->shaderProgram);
	glGetProgramiv(gl->shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		glGetProgramInfoLog(gl->shaderProgram, 512, NULL, buffer);
		printf("Shader program link failed::\n");
		printf("%s\n", buffer);
		exit(1);
	}
	if (gl->vertexShader)
		glDeleteShader(gl->vertexShader);
	if (gl->fragmentShader)
		glDeleteShader(gl->fragmentShader);
	glUseProgram(gl->shaderProgram);
}

/**
 * @brief Compile a single shader from file path
 * 
 * Helper function for enhanced rendering that compiles a shader
 * from a given file path and returns the shader ID.
 */
GLuint						compileShader(const char *shader_path, GLenum shader_type)
{
	char					*shader_source;
	char					**shader_src_ptr;
	GLuint					shader;
	
	// Read shader source from file
	shader_source = readShaderSource((char *)shader_path);
	if (!shader_source) {
		printf("\x1b[31m[%s]\x1b[0m Failed to read shader: %s\n", __FILE__, shader_path);
		return 0;
	}
	
	shader_src_ptr = &shader_source;
	
	// Compile shader
	shader = createShader(shader_type, shader_src_ptr);
	
	// Clean up
	free(shader_source);
	
	return shader;
}

/**
 * @brief Create a shader program from two compiled shaders
 * 
 * Links vertex and fragment shaders into a complete shader program.
 */
GLuint						createEnhancedProgram(GLuint vertex_shader, GLuint fragment_shader)
{
	char 					buffer[512];
	GLint 					status;
	GLuint					program;

	bzero(buffer, 512);
	program = glCreateProgram();
	
	if (vertex_shader)
		glAttachShader(program, vertex_shader);
	if (fragment_shader)
		glAttachShader(program, fragment_shader);
		
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	
	if (status != GL_TRUE)
	{
		glGetProgramInfoLog(program, 512, NULL, buffer);
		printf("\x1b[31m[%s]\x1b[0m Enhanced shader program link failed:\n%s\n", __FILE__, buffer);
		glDeleteProgram(program);
		return 0;
	}
	
	return program;
}
