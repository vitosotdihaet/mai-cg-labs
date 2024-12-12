#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>



#include "engine.hpp"



int32_t width = 800;
int32_t height = 600;

double aspectRatio = (double) width / height;

static Camera camera({ 0.0f, 0.0f, 5.0f }, { 0.0f, PI, 0.0f });
static Light l({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });


static std::vector<Model> models;

void readInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.fov += camera.fovSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.fov -= camera.fovSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.position += camera.forward * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.position -= camera.forward * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.position += camera.right * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.position -= camera.right * camera.movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) models[0].rotation.x += models[0].rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) models[0].rotation.y += models[0].rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) models[0].rotation.z += models[0].rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)        l.position.z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)      l.position.z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)      l.position.x -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)     l.position.x += 0.01;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   l.position.y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) l.position.y -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) l.color.r = std::max(0.0f, l.color.r - 0.01f);
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) l.color.g = std::max(0.0f, l.color.g - 0.01f);
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) l.color.b = std::max(0.0f, l.color.b - 0.01f);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) l.color.r = std::min(l.color.r + 0.01f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) l.color.g = std::min(l.color.g + 0.01f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) l.color.b = std::min(l.color.b + 0.01f, 1.0f);
}


void onWindowResize(GLFWwindow*, int32_t w, int32_t h) {
	glViewport(0, 0, w, h);
    width = w;
    height = h;
    aspectRatio = (double) width / height;
}

void onMouseMove(GLFWwindow*, double x, double y) {
    camera.onMouseMove(x, y);
}



int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Lab2 - 3D square pyramid", NULL, NULL);
    if (window == NULL) {
        throw std::runtime_error("could not create OpenGL window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("could not initialize GLAD");
    }

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, onWindowResize);
	glfwSetCursorPosCallback(window, onMouseMove);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);


    models.reserve(4);

    rgb model_color(1.0f, 1.0f, 1.0f);

    models.emplace_back(
        OBJModel("./l4/assets/models/monkey.obj", true),
        Shader("./l4/shaders/vs_object.glsl", "./l4/shaders/fs_light.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "camera_position", glm::value_ptr(camera.position) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "light_position", glm::value_ptr(l.position) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "light_color", glm::value_ptr(l.color) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "model_color", glm::value_ptr(model_color) },
        })),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        0.5f
    );


    models.emplace_back(
        OBJModel("./l4/assets/models/cone.obj", true),
        Shader("./l4/shaders/vs_object.glsl", "./l4/shaders/fs_colorful.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "camera_position", glm::value_ptr(camera.position) },
        })),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(-PI * 0.5f, 0.0f, 0.0f),
        0.2f
    );

    models.emplace_back(
        OBJModel("./l4/assets/models/cone.obj", true),
        Shader("./l4/shaders/vs_object.glsl", "./l4/shaders/fs_colorful.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "camera_position", glm::value_ptr(camera.position) },
        })),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        0.2f
    );

    models.emplace_back(
        OBJModel("./l4/assets/models/cone.obj", true),
        Shader("./l4/shaders/vs_object.glsl", "./l4/shaders/fs_colorful.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "camera_position", glm::value_ptr(camera.position) },
        })),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, PI * 0.5f),
        0.2f
    );



    while(!glfwWindowShouldClose(window)) {
        readInput(window);
        camera.update(aspectRatio);
        for (Model &m : models) {
            m.update();
        }


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);


        for (Model &m : models) {
            m.glDraw();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
