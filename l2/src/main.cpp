#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>


#include "matricies.hpp"
#include "shaders.hpp"


double fov = 60.0f;

double near = 1e-5f;
double far = 1e10f;


int32_t width = 800;
int32_t height = 600;

double aspectRatio = (double) width / height;


const float pi = glm::pi<float>();


namespace pyramid {
    glm::vec3 position(0.0f);
    glm::vec3 rotation(0.0f);
    double scale = 1.0;

    glm::mat4 model(0.0f);
}

namespace camera {
    glm::vec3 position(0.0f);
    glm::vec3 rotation(0.0f);
}

glm::vec2 lastMousePosition(0.0f);

glm::mat4 projection(1.0f);
glm::mat4 view(1.0f);


void readInput(GLFWwindow *window) {
    const float time = glfwGetTime();

    std::cout << "position: " << camera::position.x << ' ' << camera::position.y << ' ' << camera::position.z << ' ' << '\n';
    std::cout << "rotation: " << camera::rotation.x << ' ' << camera::rotation.y << ' ' << camera::rotation.z << ' ' << '\n';
    camera::rotation.y = glm::mod(camera::rotation.y, 2 * pi);

    if (camera::rotation.x < -pi * 0.45f) {
        camera::rotation.x = -pi * 0.45f;
    } else if (camera::rotation.x > pi * 0.45f) {
        camera::rotation.x = pi * 0.45f;
    }

    glm::vec3 cameraForward = glm::normalize(
        glm::vec3(
            -glm::cos(camera::rotation.x) * glm::sin(camera::rotation.y),
            -glm::sin(camera::rotation.x),
             glm::cos(camera::rotation.x) * glm::cos(camera::rotation.y)
        )
    );
    glm::vec3 cameraRight = glm::normalize(
        glm::cross(
            glm::vec3(0.0f, 1.0f, 0.0f), cameraForward
        )
    );
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));

    setViewMatrix(view, camera::position, camera::position + cameraForward, cameraUp);

    // setProjectionMatrix(projection, fov, near, far, aspectRatio);
    projection = glm::perspectiveFovLH(glm::radians(fov), double(width), double(height), near, far);


    pyramid::position[2] = sin(time) * 0.5f + 1.0f;
    pyramid::position[0] = cos(time) * 0.5f + 1.0f;
    // pyramid::rotation[0] = time * 0.5f;
    setModelMatrix(pyramid::model, pyramid::position, pyramid::rotation, pyramid::scale);


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera::position += cameraForward * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera::position -= cameraForward * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera::position += cameraRight * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera::position -= cameraRight * 0.1f;
}



void onWindowResize(GLFWwindow*, int32_t w, int32_t h) {
	glViewport(0, 0, w, h);
    width = w;
    height = h;
    aspectRatio = (double) width / height;
}


void onMouseMove(GLFWwindow*, double x, double y) {
	camera::rotation.y -= (x - lastMousePosition.x) * 0.001f;
	camera::rotation.x += (y - lastMousePosition.y) * 0.001f;
	lastMousePosition.x = x;
	lastMousePosition.y = y;
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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0, 0, width, height);



    // setup pyramid verticies
    GLuint pyramidVAO;
    glGenVertexArrays(1, &pyramidVAO);
    glBindVertexArray(pyramidVAO);

    GLuint pyramidVBO;
    {
        glGenBuffers(1, &pyramidVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);

        GLfloat pyramidVertices[] = {
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.0f, 1.0f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), reinterpret_cast<void*>(0));

    // shaders go brrr
    GLuint pyramidShader = compileShaderProgram("../l2/shaders/vs_pyramid.glsl", "../l2/shaders/fs_pyramid.glsl");
	glUseProgram(pyramidShader);


    GLuint pyramidModelLocation = glGetUniformLocation(pyramidShader, "model");
    GLuint pyramidProjectionLocation = glGetUniformLocation(pyramidShader, "projection");
    GLuint pyramidViewLocation = glGetUniformLocation(pyramidShader, "view");




    glUniformMatrix4fv(pyramidModelLocation, 1, GL_FALSE, glm::value_ptr(pyramid::model));
    glUniformMatrix4fv(pyramidProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(pyramidViewLocation, 1, GL_FALSE, glm::value_ptr(view));



    pyramid::position[2] = 1;
    pyramid::position[0] = 1.5;
    setModelMatrix(pyramid::model, pyramid::position, pyramid::rotation, 1);


    while(!glfwWindowShouldClose(window)) {
        readInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // NOTE: Draw the pyramid
        glBindVertexArray(pyramidVAO);
        glUseProgram(pyramidShader);
        // glEnable(GL_CULL_FACE);



        glUniformMatrix4fv(pyramidModelLocation, 1, false, glm::value_ptr(pyramid::model));
        glUniformMatrix4fv(pyramidProjectionLocation, 1, false, glm::value_ptr(projection));
        glUniformMatrix4fv(pyramidViewLocation, 1, false, glm::value_ptr(view));

        // glUniform3f(cube_view_position, camera::position.x, camera::position.y, camera::position.z);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}