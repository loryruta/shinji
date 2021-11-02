#include <cstdlib>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shinji.hpp"

void check_shader_compilation(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shader, log_length, nullptr, log.data());

		fprintf(stderr, "Shader compilation failed: %s\n", log.data());
		fflush(stderr);

		exit(3);
	}
}

GLuint create_shader(GLenum shader_type, char const* buf, size_t buf_len)
{
	GLuint shader = glCreateShader(shader_type);

	glShaderSource(shader, 1, &buf, (GLint*) &buf_len);
	glCompileShader(shader);

	check_shader_compilation(shader);

	return shader;
}

GLuint create_spirv_shader(GLenum shader_type, char const* buf, size_t buf_len)
{
	GLuint shader = glCreateShader(shader_type);

	glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, buf, (GLsizei) buf_len);
	glSpecializeShader(shader, "main", 0, nullptr, nullptr);

	check_shader_compilation(shader);

	return shader;
}

void check_program_linking(GLuint program)
{
	GLint status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetProgramInfoLog(program, log_length, &log_length, &log[0]);

		fprintf(stderr, "Program linking failed: %s\n", log.data());
		fflush(stderr);

		exit(4);
	}
}

GLuint create_test_program()
{
	GLuint program = glCreateProgram();
	GLuint v_shader, f_shader;

	{ // shaders/test.vert
		auto [src, src_len] = shinji::load_resource_from_bundle("shaders/test.vert");
		v_shader = create_shader(GL_VERTEX_SHADER, src, src_len);

		glAttachShader(program, v_shader);
	}

	{ // shaders/test.frag
		auto [src, src_len] = shinji::load_resource_from_bundle("shaders/test.frag");
		f_shader = create_shader(GL_FRAGMENT_SHADER, src, src_len);

		glAttachShader(program, f_shader);
	}

	glLinkProgram(program);
	check_program_linking(program);

	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	return program;
}

GLuint create_test_spirv_program()
{
	GLuint program = glCreateProgram();

	GLuint v_shader, f_shader;

	{ // shaders/test.vert.spirv
		auto [src, src_len] = shinji::load_resource_from_bundle(".spv/test.vert.spv");
		v_shader = create_spirv_shader(GL_VERTEX_SHADER, src, src_len);

		glAttachShader(program, v_shader);
	}

	{ // shaders/test.frag.spirv
		auto [src, src_len] = shinji::load_resource_from_bundle(".spv/test.frag.spv");
		f_shader = create_spirv_shader(GL_FRAGMENT_SHADER, src, src_len);

		glAttachShader(program, v_shader);
	}

	glLinkProgram(program);
	check_program_linking(program);

	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	return program;
}

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Couldn't initialize GLFW.");
		fflush(stderr);

		return 1;
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(512, 512, "shinji", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		fprintf(stderr, "Couldn't initialize GLAD.\n");
		fflush(stderr);

		return 2;
	}

	GLuint program;

	program = create_test_program();
	glDeleteProgram(program);
	printf("Test program created\n");

	program = create_test_spirv_program();
	glDeleteProgram(program);
	printf("Test SPIR-V program created\n");

	glfwTerminate();

	return 0;
}
