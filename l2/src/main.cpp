#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>


#include "matricies.hpp"
#include "shaders.hpp"



int32_t width = 800;
int32_t height = 600;

double aspectRatio = (double) width / height;


const float pi = glm::pi<float>();


namespace pyramid {
    glm::vec3 position(1.0f, 0.0f, 1.0f);
    glm::vec3 origin(0.0f, 0.5f, 0.0f);
    glm::vec3 rotation(0.0f);
    float scale = 1.0f;

    const float movementSpeed = 0.1f;
    const float rotationSpeed = 0.05f;

    glm::mat4 model(0.0f);
}

namespace camera {
    glm::mat4 view(1.0f);
    glm::mat4 projection(0.0f);

    glm::vec3 position(0.0f);
    glm::vec3 rotation(0.0f);

    const float minXRotation = -pi * 0.49f;
    const float maxXRotation =  pi * 0.49f;

    float fov = 60.0f;

    const float minFov = 1.0f;
    const float maxFov = 359.0f;

    const float movementSpeed = 0.1f;
    const float fovSpeed = 0.1f;

    const float near = 1e-5f;
    const float far = 1e10f;

    glm::vec3 forward(0.0f);
    glm::vec3 right(0.0f);
    glm::vec3 up(0.0f);

    void update() {
        camera::rotation.y = glm::mod(camera::rotation.y, 2 * pi);

        if (camera::rotation.x < camera::minXRotation) {
            camera::rotation.x = camera::minXRotation;
        } else if (camera::rotation.x > camera::maxXRotation) {
            camera::rotation.x = camera::maxXRotation;
        }

        if (camera::fov < camera::minFov) {
            camera::fov = camera::minFov;
        } else if (camera::fov > camera::maxFov) {
            camera::fov = camera::maxFov;
        }

        camera::forward = glm::normalize(glm::vec3(
            -glm::cos(camera::rotation.x) * glm::sin(camera::rotation.y),
            -glm::sin(camera::rotation.x),
            glm::cos(camera::rotation.x) * glm::cos(camera::rotation.y)
        ));
        camera::right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera::forward));
        camera::up = glm::normalize(glm::cross(camera::forward, camera::right));
    }
}

glm::vec2 lastMousePosition(0.0f);



void readInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera::fov += camera::fovSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera::fov -= camera::fovSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera::position += camera::forward * camera::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera::position -= camera::forward * camera::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera::position += camera::right * camera::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera::position -= camera::right * camera::movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) pyramid::rotation.x += pyramid::rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) pyramid::rotation.y += pyramid::rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) pyramid::rotation.z += pyramid::rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)        pyramid::position.z += pyramid::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)      pyramid::position.z -= pyramid::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)      pyramid::position.x -= pyramid::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)     pyramid::position.x += pyramid::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   pyramid::position.y += pyramid::movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) pyramid::position.y -= pyramid::movementSpeed;
}


void update() {
    camera::update();
    setViewMatrix(camera::view, camera::position, camera::position + camera::forward, camera::up);
    setPerspectiveProjectionMatrix(camera::projection, camera::fov, camera::near, camera::far, aspectRatio);    
    setModelMatrix(pyramid::model, pyramid::origin, pyramid::position, pyramid::rotation, pyramid::scale);
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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0, 0, width, height);



    // setup pyramid
    GLuint pyramidVAO;
    glGenVertexArrays(1, &pyramidVAO);
    glBindVertexArray(pyramidVAO);

    // setup pyramid verticies
    GLuint pyramidVBO;
    glGenBuffers(1, &pyramidVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    GLfloat pyramidVertices[] = {
            -0.5f, 0.0f, -0.5f,
            -0.5f, 0.0f,  0.5f,
             0.5f, 0.0f,  0.5f,
             0.5f, 0.0f, -0.5f,
             0.0f, 1.0f,  0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), reinterpret_cast<void*>(0));

    // setup pyramid vertex indecies
    GLuint pyramidIBO;
    glGenBuffers(1, &pyramidIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO);
    GLuint pyramidIndecies[] = {
        3, 0, 1,
        3, 1, 2,
        4, 1, 0,
        4, 2, 1,
        4, 3, 2,
        4, 0, 3,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndecies), pyramidIndecies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, false, 3 * sizeof(GLuint), reinterpret_cast<void*>(0));


    // shaders go brrr
    GLuint pyramidShader = compileVsFsProgram("./l2/shaders/vs_pyramid.glsl", "./l2/shaders/fs_pyramid.glsl");
	glUseProgram(pyramidShader);

    GLuint pyramidModelLocation = glGetUniformLocation(pyramidShader, "model");
    GLuint pyramidProjectionLocation = glGetUniformLocation(pyramidShader, "projection");
    GLuint pyramidViewLocation = glGetUniformLocation(pyramidShader, "view");

    GLuint pyramidCameraPosition = glGetUniformLocation(pyramidShader, "camera_position");


    while(!glfwWindowShouldClose(window)) {
        readInput(window);
        update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(pyramidVAO);
        glUseProgram(pyramidShader);



        glUniformMatrix4fv(pyramidModelLocation, 1, false, glm::value_ptr(pyramid::model));
        glUniformMatrix4fv(pyramidProjectionLocation, 1, false, glm::value_ptr(camera::projection));
        glUniformMatrix4fv(pyramidViewLocation, 1, false, glm::value_ptr(camera::view));
        glUniform3f(pyramidCameraPosition, camera::position.x, camera::position.y, camera::position.z);

        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndecies) / sizeof(pyramidIndecies[0]), GL_UNSIGNED_INT, nullptr);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}