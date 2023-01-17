#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader.h"
#include "model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Display size
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// Camera (with initial position)
Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));



glm::vec3 lightPos(0.0f, 15.0f, 0.0f);



int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BillardGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // Shaders (as for now, roomShader=tableShader but duplicated for future proofing)
    Shader tableShader("../models/table/tableShader.vs", "../models/table/tableShader.fs");
    Shader roomShader("../models/room/roomShader.vs", "../models/room/roomShader.fs");
    Shader reflectiveBallShader("../models/balls/ballShader.vs", "../models/balls/ballShader.fs");

    // Models
    Model tableModel("../models/table/pooltable.obj");
    Model roomModel("../models/room/room.obj");
    Model reflectiveBallModel("../models/balls/sphere.obj");

    // Position the light
    const glm::vec3 light_pos = glm::vec3(0.0, 3.0, 0.0);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {

        // input
        processInput(window);

        // render
        glClearColor(0.76f, 0.88f, 1.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tableShader.use();
        roomShader.use();
        reflectiveBallShader.use();

        // Set everything for the table
        // light properties
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        tableShader.setVector3f("light.ambient", ambientColor * 7.0f); // multiplied to increase the intensity
        tableShader.setVector3f("light.diffuse", diffuseColor * 7.0f);
        tableShader.setVector3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        tableShader.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
        tableShader.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        tableShader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        tableShader.setFloat("material.shininess", 32.0f);


        // Set everything for the room
        // light properties
        roomShader.setVector3f("light.ambient", ambientColor * 7.0f);
        roomShader.setVector3f("light.diffuse", diffuseColor * 7.0f);
        roomShader.setVector3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        roomShader.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
        roomShader.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        roomShader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        roomShader.setFloat("material.shininess", 32.0f);


        // Set everything for the reflective ball
        // light properties
        reflectiveBallShader.setVector3f("light.ambient", ambientColor * 7.0f);
        reflectiveBallShader.setVector3f("light.diffuse", diffuseColor * 7.0f);
        reflectiveBallShader.setVector3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        reflectiveBallShader.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
        reflectiveBallShader.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        reflectiveBallShader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        reflectiveBallShader.setFloat("material.shininess", 32.0f);


        // view & projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        tableShader.setMatrix4("projection", projection);
        tableShader.setMatrix4("view", view);

        roomShader.setMatrix4("projection", projection);
        roomShader.setMatrix4("view", view);

        reflectiveBallShader.setMatrix4("projection", projection);
        reflectiveBallShader.setMatrix4("view", view);


        tableShader.setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
        tableShader.setVector3f("light.position", lightPos);
        tableShader.setVector3f("viewPos", camera.Position);

        roomShader.setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
        roomShader.setVector3f("light.position", lightPos);
        roomShader.setVector3f("viewPos", camera.Position);

        reflectiveBallShader.setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
        reflectiveBallShader.setVector3f("light.position", lightPos);
        reflectiveBallShader.setVector3f("viewPos", camera.Position);
        reflectiveBallShader.setFloat("Material.refractionIndex", 0.2f);

        // Render the pool table
        glm::mat4 pooltable = glm::mat4(1.0f);
        pooltable = glm::translate(pooltable, glm::vec3(0.0f, 0.0f, 0.0f)); // position in the scene
        pooltable = glm::scale(pooltable, glm::vec3(10.0f, 10.0f, 10.0f));     // scale
        tableShader.setMatrix4("model", pooltable);
        tableModel.Draw(tableShader);

        // Render the room
        glm::mat4 room = glm::mat4(1.0f);
        room = glm::translate(room, glm::vec3(0.0f, 0.0f, 0.0f)); // position in the scene
        room = glm::scale(room, glm::vec3(15.0f, 15.0f, 15.0f));     // scale
        roomShader.setMatrix4("model", room);
        roomModel.Draw(roomShader);

        // Render the reflective ball
        glm::mat4 reflectiveBall = glm::mat4(1.0f);
        reflectiveBall = glm::translate(reflectiveBall, glm::vec3(4.0f, 2.9f, 1.5f)); // position in the scene
        reflectiveBall = glm::scale(reflectiveBall, glm::vec3(0.3f, 0.3f, 0.3f));     // scale
        reflectiveBallShader.setMatrix4("model", reflectiveBall);
        reflectiveBallModel.Draw(reflectiveBallShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
