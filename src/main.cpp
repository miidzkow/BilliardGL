#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader.h"
#include "object.h"
#include "model.h"


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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

    Shader ourShader("pooltable.vs", "pooltable.fs");


    char path[] = PATH_TO_OBJECTS "/pooltable.obj";

    Model ourModel(path);



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

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        view = camera.GetViewMatrix();
        double now = glfwGetTime();


        // Use the shader Class to send the uniform
        ourShader.use();

        ourShader.setMatrix4("M", model);
        ourShader.setMatrix4("V", view);
        ourShader.setMatrix4("P", perspective);


        ourModel.Draw(ourShader);

        fps(now);
        glfwSwapBuffers(window);
        glfwPollEvents();

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


