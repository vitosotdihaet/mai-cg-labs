#include <exception>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>

#include <glad/glad.h>

#include <GLFW/glfw3.h>



GLuint setupShader(const std::string_view shader_path, GLint type) {
    GLuint shader = glCreateShader(type);
    {
    	std::filesystem::path path = shader_path;
		std::ifstream file(path);

		const size_t fileSize = std::filesystem::file_size(path);
		std::string shaderCode(fileSize, '\0');

		file.read(shaderCode.data(), fileSize);

		const GLchar *const code_cstr = shaderCode.c_str();
		glShaderSource(shader, 1, &code_cstr, nullptr);

		glCompileShader(shader);

		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string log(logLength, '\0');
			glGetShaderInfoLog(shader, logLength, nullptr, log.data());
			throw std::runtime_error(log);
		}
	}

    return shader;
}


static GLuint compileShaderProgram(const std::string_view vertex_shader_path, const std::string_view fragment_shader_path) {
    GLuint vertex_shader = setupShader(vertex_shader_path, GL_VERTEX_SHADER);
	GLuint fragment_shader = setupShader(fragment_shader_path, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
    {
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string log(logLength, '\0');
			glGetProgramInfoLog(program, logLength, nullptr, log.data());
			throw std::runtime_error(log);
		}
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

    return program;
}


