#include <iostream>
#include <cinttypes>

#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>

#include "model.hpp"



OBJModel::OBJModel(const std::string_view path, bool isCW = false) : isCW(isCW) {
    std::filesystem::path objFilePath(path);
    std::ifstream objFile(objFilePath);

    std::string currentLine;

    while (std::getline(objFile, currentLine)) {
        if (currentLine.length() == 0) { continue; }
        std::stringstream lineStream(currentLine);
        std::string prefix;

        lineStream >> prefix;

        if (prefix == "#") {
            continue;
        } else if (prefix == "v") { // vertices
            glm::vec4 vertex;
            vertex.w = 1.0f;
            sscanf(currentLine.c_str(), "v %f %f %f %f", &vertex.x, &vertex.y, &vertex.z, &vertex.w);
            this->vertices.push_back(vertex);
        } else if (prefix == "vt") { // texture
            glm::vec3 uv;
            sscanf(currentLine.c_str(), "vt %f %f %f", &uv.x, &uv.y, &uv.z);
            this->uvs.push_back(uv);
        } else if (prefix == "vn") { // normals
            glm::vec3 normal;
            sscanf(currentLine.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            this->normals.push_back(normal);
        } else if (prefix == "f") { // mapping to faces
            std::string element;
            while (lineStream >> element) {
                size_t slashCount = 0;
                for (char e : element) { if (e == '/') { slashCount++; } }

                if (slashCount == 0) { // only vertex
                    uint64_t vertexIndex = std::stoull(element);
                    this->vertexIndeces.push_back(vertexIndex);
                } else if (slashCount == 1) { // vertex and a texture
                    uint64_t vertexIndex;
                    uint64_t uvIndex;

                    sscanf(element.c_str(), "%" SCNu64 "/%" SCNu64, &vertexIndex, &uvIndex);

                    this->vertexIndeces.push_back(vertexIndex);
                    this->uvIndeces.push_back(uvIndex);
                } else if (slashCount == 2) { // vertex, texture and a normal
                    uint64_t vertexIndex;
                    uint64_t uvIndex;
                    uint64_t normalIndex;

                    sscanf(element.c_str(), "%" SCNu64 "/%" SCNu64 "/%" SCNu64, &vertexIndex, &uvIndex, &normalIndex);

                    this->vertexIndeces.push_back(vertexIndex);
                    this->uvIndeces.push_back(uvIndex);
                    this->normalIndeces.push_back(normalIndex);
                } else {
                    std::cout << "Could not parse this shiet: " << element << '\n';
                }
            }
        } else {
            std::cout << "Parsing of prefix " << prefix << " is unimplemented: " << currentLine << '\n';
        }
    }
}



Model::Model(glm::mat4 &model, OBJModel obj, Shader shader) : model(model), obj(obj), shader(shader) {
    this->glSetup();
    this->shader.addVariable(GetShaderVariable { ShaderVariableType::MATRIX4, "model", glm::value_ptr(this->model) });
}

void Model::glSetup() {
    std::vector<float> vertexBufferData;

    bool hasUv = !this->obj.uvs.empty();
    bool hasNormals = !this->obj.normals.empty();

    for (size_t i = 0; i < this->obj.vertexIndeces.size(); ++i) {
        size_t vertexIndex = this->obj.vertexIndeces[i] - 1;

        const glm::vec4 &vertex = this->obj.vertices[vertexIndex];
        vertexBufferData.push_back(vertex.x);
        vertexBufferData.push_back(vertex.y);
        vertexBufferData.push_back(vertex.z);

        if (hasUv) {
            size_t uvIndex = this->obj.uvIndeces[i] - 1;
            const glm::vec3 &uv = this->obj.uvs[uvIndex];
            vertexBufferData.push_back(uv.x);
            vertexBufferData.push_back(uv.y);
            vertexBufferData.push_back(uv.z);
        }

        if (hasNormals) {
            size_t normalIndex = this->obj.normalIndeces[i] - 1;
            const glm::vec3 &normal = this->obj.normals[normalIndex];
            vertexBufferData.push_back(normal.x);
            vertexBufferData.push_back(normal.y);
            vertexBufferData.push_back(normal.z);
        }
    }

    this->indecesCount = this->obj.vertexIndeces.size();

    size_t elementSize = sizeof(vertexBufferData[0]);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * elementSize, vertexBufferData.data(), GL_STATIC_DRAW);

    GLint stride = (3 + (hasUv ? 3 : 0) + (hasNormals ? 3 : 0)) * elementSize;
    size_t offset = 0;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += elementSize * 3;

    if (hasUv) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += elementSize * 3;
    }

    if (hasNormals) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}


void Model::update() {
    this->setModel();
}

void Model::setModel() {
    glm::mat4 t(1.0f);
    t[3] = glm::vec4(this->position.x, this->position.y, this->position.z, 1.0f);

    const double xRotation = rotation.x;
    float xCosine = cos(xRotation);
    float xSine = sin(xRotation);
    glm::mat4 xRotationMatrix(1.0f);
    xRotationMatrix[1][1] = xCosine; xRotationMatrix[1][2] = -xSine;
    xRotationMatrix[2][1] = xSine;   xRotationMatrix[2][2] = xCosine;

    const double yRotation = rotation.y;
    float yCosine = cos(yRotation);
    float ySine = sin(yRotation);
    glm::mat4 yRotationMatrix(1.0f);
    yRotationMatrix[0][0] = yCosine; yRotationMatrix[0][2] = ySine;
    yRotationMatrix[2][0] = -ySine;  yRotationMatrix[2][2] = yCosine;

    const double zRotation = rotation.z;
    float zCosine = cos(zRotation);
    float zSine = sin(zRotation);
    glm::mat4 zRotationMatrix(1.0f);
    zRotationMatrix[0][0] = zCosine; zRotationMatrix[0][1] = -zSine;
    zRotationMatrix[1][0] = zSine;   zRotationMatrix[1][1] = zCosine;

    glm::mat4 r = xRotationMatrix * yRotationMatrix * zRotationMatrix;

    glm::mat4 s(scale);
    s[3][3] = 1.0f;

    this->model = t * r * s;
}

void Model::glDraw() {
    glBindVertexArray(this->VAO);

    this->shader.glUse();

    if (this->obj.isCW) {
        glFrontFace(GL_CW);
    } else {
        glFrontFace(GL_CCW);
    }

    glDrawArrays(GL_TRIANGLES, 0, this->indecesCount);
}