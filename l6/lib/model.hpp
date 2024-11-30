#include <exception>
#include <filesystem>
#include <sstream>
#include <string_view>
#include <fstream>
#include <stdlib.h>

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    OBJModel();
    OBJModel(const std::string_view path, bool isCW);
};


class Model {
private:
    glm::mat4 modelMatrix;

    OBJModel obj;
    Shader shader;

    GLuint VAO;
    size_t indecesCount;

public:


private:
    void glSetup();

public:
    Model(glm::mat4 &modelMatrix, OBJModel obj, Shader shader);
    void glDraw();
};