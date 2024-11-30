#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>



#include "model.hpp"
#include "shader.hpp"
#include "camera.hpp"



int32_t width = 800;
int32_t height = 600;

double aspectRatio = (double) width / height;

static Camera camera;



void readInput(GLFWwindow *window, Model model) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.fov += camera.fovSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.fov -= camera.fovSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.position += camera.forward * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.position -= camera.forward * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.position += camera.right * camera.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.position -= camera.right * camera.movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) model.rotation.x += model.rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) model.rotation.y += model.rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) model.rotation.z += model.rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)        model.position.z += model.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)      model.position.z -= model.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)      model.position.x -= model.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)     model.position.x += model.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   model.position.y += model.movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) model.position.y -= model.movementSpeed;
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


    Model sphere(
        sphere.model,
        OBJModel("./l6/assets/models/sphere.obj", true),
        Shader("./l6/shaders/vs_object.glsl", "./l6/shaders/fs_colorful.glsl", std::vector<GetShaderVariable>({
            GetShaderVariable { ShaderVariableType::MATRIX4, "view", glm::value_ptr(camera.view) },
            GetShaderVariable { ShaderVariableType::MATRIX4, "projection", glm::value_ptr(camera.projection) },
        }))
    );


    while(!glfwWindowShouldClose(window)) {
        readInput(window, sphere);

        camera.update(aspectRatio);
        sphere.update();


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);


        sphere.glDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
