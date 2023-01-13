#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "camera.h"
#include "shader.h"
#include "object.h"


const int width = 2000;
const int height = 2000;



void processInput(GLFWwindow* window);



Camera camera(glm::vec3(0.0, 0.0, 0.1));


int main(int argc, char* argv[])
{
    //Boilerplate
    //Create the OpenGL context
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise GLFW \n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    //Create the window
    GLFWwindow* window = glfwCreateWindow(width, height, "BillardGL", nullptr, nullptr);
    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window\n");
    }

    glfwMakeContextCurrent(window);

    //load openGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);


    const std::string sourceV = "#version 330 core\n"
                                "in vec3 position; \n"
                                "in vec2 tex_coord; \n"
                                "in vec3 normal; \n"
                                "out vec4 v_col; \n"
                                "out vec2 v_t; \n"
                                "uniform mat4 M; \n"
                                "uniform mat4 V; \n"
                                "uniform mat4 P; \n"
                                " void main(){ \n"
                                "gl_Position = P*M*V*vec4(position, 1);\n"
                                "v_col = vec4(normal*0.5 + 0.5, 1.0);\n"
                                "v_t = tex_coord; \n"
                                "}\n";
    const std::string sourceF = "#version 330 core\n"
                                "out vec4 FragColor;"
                                "precision mediump float; \n"
                                "in vec4 v_col; \n"
                                "in vec2 v_t; \n"
                                "void main() { \n"
                                "FragColor = v_col*(1.0-v_t.y); \n"
                                "} \n";


    Shader shader(sourceV, sourceF);

    char path[] = PATH_TO_OBJECTS "/table.obj";

    Object cube(path);
    cube.makeObject(shader);



    double prev = 0;
    int deltaFrame = 0;
    //fps function
    auto fps = [&](double now) {
        double deltaTime = now - prev;
        deltaFrame++;
        if (deltaTime > 0.5) {
            prev = now;
            const double fpsCount = (double)deltaFrame / deltaTime;
            deltaFrame = 0;
            std::cout << "\r FPS: " << fpsCount;
        }
    };


    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.5, 0.5, -1.0));
    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));


    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 perspective = camera.GetProjectionMatrix();


    glfwSwapInterval(1);
    //Rendering

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        view = camera.GetViewMatrix();
        glfwPollEvents();
        double now = glfwGetTime();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Use the shader Class to send the uniform
        shader.use();

        shader.setMatrix4("M", model);
        shader.setMatrix4("V", view);
        shader.setMatrix4("P", perspective);


        cube.draw();

        fps(now);
        glfwSwapBuffers(window);
    }

    //clean up ressource
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window) {
    // Use the cameras class to change the parameters of the camera
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(LEFT, 0.1);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(RIGHT, 0.1);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(FORWARD, 0.1);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(BACKWARD, 0.1);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboardRotation(1, 0.0,1);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboardRotation(-1, 0.0,1);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboardRotation(0.0, 1.0, 1);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboardRotation(0.0, -1.0, 1);


}


