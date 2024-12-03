#include <memory>

#include "shader.hpp"



Shader::Shader(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath, const std::vector<GetShaderVariable> &variables) {
    GLuint vertexShader = this->setupShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = this->setupShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    this->program = glCreateProgram();
    {
        glAttachShader(this->program, vertexShader);
        glAttachShader(this->program, fragmentShader);

        glLinkProgram(this->program);

        GLint logLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0) {
            std::string log(logLength, '\0');
            glGetProgramInfoLog(this->program, logLength, nullptr, log.data());
            throw std::runtime_error(log);
        }
    }

    glDetachShader(this->program, vertexShader);
    glDetachShader(this->program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(this->program);

    this->variables.reserve(variables.size());
    for (const GetShaderVariable &variable : variables) {
        this->addVariable(variable);
    }
}

GLuint Shader::setupShader(const std::string_view shader_path, GLint type) {
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

void Shader::addVariable(const GetShaderVariable &variable) {
    this->variables.push_back(ShaderVariable { variable.type, glGetUniformLocation(this->program, variable.name.c_str()), variable.data });
}

// #include <iostream>
void Shader::glUse() {
    // std::cout << "prog: " << this->program << '\n';
    glUseProgram(this->program);

    for (const ShaderVariable &variable : this->variables) {
        // std::cout << "var = " << variable.data << '\n';
        switch (variable.type) { // TODO: add other types
            case ShaderVariableType::VECTOR2:
                glUniform2f(variable.location, ((GLfloat*) variable.data)[0], ((GLfloat*) variable.data)[1]);
                break;
            case ShaderVariableType::VECTOR3:
                glUniform3f(variable.location, ((GLfloat*) variable.data)[0], ((GLfloat*) variable.data)[1], ((GLfloat*) variable.data)[2]);
                break;
            case ShaderVariableType::VECTOR4:
                glUniform4f(variable.location, ((GLfloat*) variable.data)[0], ((GLfloat*) variable.data)[1], ((GLfloat*) variable.data)[2], ((GLfloat*) variable.data)[3]);
                break;
            case ShaderVariableType::MATRIX2:
                glUniformMatrix2fv(variable.location, 1, false, (GLfloat*) variable.data);
                break;
            case ShaderVariableType::MATRIX3:
                glUniformMatrix3fv(variable.location, 1, false, (GLfloat*) variable.data);
                break;
            case ShaderVariableType::MATRIX4:
                // std::cout << "shader =\n";
                // std::cout << ((GLfloat*) variable.data)[0] << ' ' << ((GLfloat*) variable.data)[1] << ' ' << ((GLfloat*) variable.data)[2] << ' ' << ((GLfloat*) variable.data)[3] << '\n'; 
                // std::cout << ((GLfloat*) variable.data)[4] << ' ' << ((GLfloat*) variable.data)[5] << ' ' << ((GLfloat*) variable.data)[6] << ' ' << ((GLfloat*) variable.data)[7] << '\n'; 
                // std::cout << ((GLfloat*) variable.data)[8] << ' ' << ((GLfloat*) variable.data)[9] << ' ' << ((GLfloat*) variable.data)[10] << ' ' << ((GLfloat*) variable.data)[11] << '\n'; 
                // std::cout << ((GLfloat*) variable.data)[12] << ' ' << ((GLfloat*) variable.data)[13] << ' ' << ((GLfloat*) variable.data)[14] << ' ' << ((GLfloat*) variable.data)[15] << '\n';
                glUniformMatrix4fv(variable.location, 1, false, (GLfloat*) variable.data);
                break;
        }
    }
}