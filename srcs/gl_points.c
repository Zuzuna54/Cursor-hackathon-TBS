#include "morphosis.h"

void						gl_retrieve_tris(t_data *data)
{
	uint					i;
	uint 					j;

	i = 0;
	j = 0;
	if (!(data->gl->tris = (float *)malloc(data->gl->num_pts * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);

	bzero(data->gl->tris, data->gl->num_pts);
	while (j < data->gl->num_pts)
	{
		for (int c = 0; c < 3; c++)
		{
			data->gl->tris[j++] = data->triangles[i][c].x;
			data->gl->tris[j++] = data->triangles[i][c].y;
			data->gl->tris[j++] = data->triangles[i][c].z;
		}
		i++;
	}
}

void						gl_set_attrib_ptr(t_gl *gl, char *attrib_name, GLint num_vals, int stride, int offset)
{
	GLuint 					attrib;

	attrib = glGetAttribLocation(gl->shaderProgram, attrib_name);
	glVertexAttribPointer(attrib, num_vals, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(offset * sizeof(float)));
	glEnableVertexAttribArray(attrib);
}
