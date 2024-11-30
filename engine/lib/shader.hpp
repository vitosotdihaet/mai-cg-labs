#pragma once

#include <exception>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>

#include <glad/glad.h>

#include <GLFW/glfw3.h>



enum ShaderVariableType {
	VECTOR2,
	VECTOR3,
	VECTOR4,
	MATRIX2,
	MATRIX3,
	MATRIX4,
};



struct GetShaderVariable {
	ShaderVariableType type;
	std::string name;
	const void *data;
};

struct ShaderVariable {
	ShaderVariableType type;
	GLint location;
	const void *data;
};



class Shader {
private:
	GLuint program;
	std::vector<ShaderVariable> variables;

public:
	Shader(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath, const std::vector<GetShaderVariable> &variables);
	GLuint setupShader(const std::string_view shader_path, GLint type);

	void addVariable(const GetShaderVariable &variable);

	// evaluate variables
	void glUse();
};
