#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>



#include "engine.hpp"



int32_t width = 800;
int32_t height = 600;

double aspectRatio = (double) width / height;

static Camera camera;

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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)        models[0].position.z += models[0].movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)      models[0].position.z -= models[0].movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)      models[0].position.x -= models[0].movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)     models[0].position.x += models[0].movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   models[0].position.y += models[0].movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) models[0].position.y -= models[0].movementSpeed;
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



    models.emplace_back(
        OBJModel("./l6/assets/models/sphere.obj", true),
        Shader("./l6/shaders/vs_object.glsl", "./l6/shaders/fs_dontcomeclose.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
            GetShaderVariable { ShaderVariableType::VECTOR3, "camera_position", glm::value_ptr(camera.position) },
        })),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
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
