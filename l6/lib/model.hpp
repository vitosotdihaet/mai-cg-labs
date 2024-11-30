#pragma once

#include <exception>
#include <filesystem>
#include <sstream>
#include <string_view>
#include <fstream>

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GLFW/glfw3.h>


#include "shader.hpp"



struct OBJModel {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> uvs;
    std::vector<glm::vec3> normals;

    std::vector<size_t> vertexIndeces;
    std::vector<size_t> uvIndeces;
    std::vector<size_t> normalIndeces;

    bool isCW = false;

    OBJModel(const std::string_view path, bool isCW);
};


class Model {
private:
    OBJModel obj;
    Shader shader;

    GLuint VAO;
    size_t indecesCount;

public:
    glm::mat4 model;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    float scale = 1.0f;

    const float movementSpeed = 0.1f;
    const float rotationSpeed = 0.05f;

private:
    void glSetup();

public:
    Model(glm::mat4 &model, OBJModel obj, Shader shader);
    void update();
    void setModel();
    void glDraw();
};