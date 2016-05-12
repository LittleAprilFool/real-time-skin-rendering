#include "stdafx.h"
#include "Beckmann.h"

Beckmann::Beckmann()
{
}


Beckmann::~Beckmann()
{
}

void Beckmann::Precompute(int w, int h)
{
	width = w;
	height = h;
	Init();
}

void Beckmann::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao_handle);
	glUseProgram(shader_beckmann_ID);
	glDrawArrays(GL_QUADS, 0, width * height * 8);
}

void Beckmann::Init() 
{
	//attach VAO VBO
	glGenBuffers(1, &vbo_handle);

	GLfloat *texcoord_data = new GLfloat[width * height * 8];

	int data_sum = 0;

	float delta_w = 1.0 / width;
	float delta_h = 1.0 / height;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			texcoord_data[data_sum + 0] = i * delta_w;
			texcoord_data[data_sum + 1] = j * delta_h;
			texcoord_data[data_sum + 2] = (i + 1) * delta_w;
			texcoord_data[data_sum + 3] = j * delta_h;
			texcoord_data[data_sum + 4] = (i + 1) * delta_w;
			texcoord_data[data_sum + 5] = (j + 1) * delta_h;
			texcoord_data[data_sum + 6] = i * delta_w;
			texcoord_data[data_sum + 7] = (j + 1) * delta_h;
			data_sum += 8;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, data_sum * sizeof(GLfloat), texcoord_data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	//compile shader
	shader_beckmann_ID = LoadShader(shader_beckmann, "vbeckmann.glsl", "fbeckmann.glsl");
}

GLuint Beckmann::LoadShader(ShaderProgram* shader, const char* vertex_shader_filename, const char* fragment_shader_filename)
{
	shader = new ShaderProgram;
	GLuint shader_ID;
	shader_ID = shader->InitShader(vertex_shader_filename, fragment_shader_filename);
	return shader_ID;
}