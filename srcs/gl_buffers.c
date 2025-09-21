#include "morphosis.h"

void 						createVBO(t_gl *gl, GLsizeiptr size, GLfloat *points)
{
	glGenBuffers(1, &gl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_DYNAMIC_DRAW);
}

void						createVAO(t_gl *gl)
{
	glGenVertexArrays(1, &gl->vao);
	glBindVertexArray(gl->vao);
}

/**
 * @brief Optimized VBO creation with better buffer usage hints
 * 
 * Uses GL_STATIC_DRAW for better performance when data doesn't change
 * and implements proper error checking.
 */
void 						createVBO_optimized(t_gl *gl, GLsizeiptr size, GLfloat *points)
{
	printf("\x1b[36m[%s]\x1b[0m Creating optimized VBO: %.2f MB\n", 
		   __FILE__, (float)size / (1024.0f * 1024.0f));
		   
	glGenBuffers(1, &gl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	
	// Use GL_STATIC_DRAW for better performance (data doesn't change after upload)
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
	
	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("\x1b[31m[%s]\x1b[0m OpenGL error in VBO creation: 0x%x\n", __FILE__, error);
	}
}

/**
 * @brief Optimized VBO update for dynamic data
 * 
 * Uses glBufferSubData for efficient updates when buffer size doesn't change.
 */
void						updateVBO_optimized(t_gl *gl, GLsizeiptr size, GLfloat *points)
{
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	
	// More efficient than glBufferData for updates
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, points);
	
	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("\x1b[31m[%s]\x1b[0m OpenGL error in VBO update: 0x%x\n", __FILE__, error);
	}
}
