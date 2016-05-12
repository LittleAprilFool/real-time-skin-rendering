#include "stdafx.h"
#include "Shader.h"


ShaderProgram::ShaderProgram()
{
}


ShaderProgram::~ShaderProgram()
{
}

GLuint ShaderProgram::InitShader(const char* vertex_shader_file, const char* fragment_shader_file) {
	//Create the shaders
	GLuint vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read the Vertex Shader code from the file
	std::string vertex_shader_code;
	std::ifstream vertex_shader_stream(vertex_shader_file, std::ios::in);

	if (vertex_shader_stream.is_open()) {
		std::string line = "";
		while (getline(vertex_shader_stream, line)) vertex_shader_code += "\n" + line;
		vertex_shader_stream.close();
	}
	else {
		std::cerr << "Failed to read " << vertex_shader_file << std::endl;
		exit(EXIT_FAILURE);
	}

	//Read the Fragment Shader code from the file
	std::string fragment_shader_code;
	std::ifstream fragment_shader_stream(fragment_shader_file, std::ios::in);

	if (fragment_shader_stream.is_open()) {
		std::string line = "";
		while (getline(fragment_shader_stream, line)) fragment_shader_code += "\n" + line;
		fragment_shader_stream.close();
	}
	else {
		std::cerr << "Failed to read" << fragment_shader_file << std::endl;
	}

	GLint result = GL_FALSE;
	int info_log_length;

	//Compile Vertex Shader
	char const * vertex_source_pointer = vertex_shader_code.c_str();
	glShaderSource(vertex_shader_ID, 1, &vertex_source_pointer, NULL);
	glCompileShader(vertex_shader_ID);

	//Check Vertex Shader
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &info_log_length);

	if (info_log_length > 0) {
		std::vector<char> vertex_shader_error_message(info_log_length + 1);
		glGetShaderInfoLog(vertex_shader_ID, info_log_length, NULL, &vertex_shader_error_message[0]);
		std::cerr << "Vertex Shader Error Message:" << &vertex_shader_error_message[0] << std::endl;
	}

	//Compile Fragment Shader
	char const* fragment_source_pointer = fragment_shader_code.c_str();
	glShaderSource(fragment_shader_ID, 1, &fragment_source_pointer, NULL);
	glCompileShader(fragment_shader_ID);

	//Check Fragment Shader
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_ID, GL_INFO_LOG_LENGTH, &info_log_length);

	if (info_log_length > 0) {
		std::vector<char> fragment_shader_error_message(info_log_length + 1);
		glGetShaderInfoLog(fragment_shader_ID, info_log_length, NULL, &fragment_shader_error_message[0]);
		std::cerr << "Fragment Shader Error Message:" << &fragment_shader_error_message[0] << std::endl;
	}

	//Link the program
	GLuint program_ID = glCreateProgram();
	glAttachShader(program_ID, vertex_shader_ID);
	glAttachShader(program_ID, fragment_shader_ID);
	glLinkProgram(program_ID);

	//Check the program
	glGetProgramiv(program_ID, GL_LINK_STATUS, &result);
	glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> program_error_message(info_log_length + 1);
		glGetProgramInfoLog(program_ID, info_log_length, NULL, &program_error_message[0]);
		std::cerr << "Program Error Message:" << &program_error_message << std::endl;
	}

	glDetachShader(program_ID, vertex_shader_ID);
	glDetachShader(program_ID, fragment_shader_ID);

	glDeleteShader(vertex_shader_ID);
	glDeleteShader(fragment_shader_ID);
	
	return program_ID;
}