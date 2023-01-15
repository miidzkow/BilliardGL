#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader.h"
#include "model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1500;

// camera
Camera camera(glm::vec3(0.0f, 8.0f, 17.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;



glm::vec3 lightPos(0.0f, 15.0f, 0.0f);



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BillardGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("../models/room/roomShader.vs", "../models/room/roomShader.fs");

    Shader roomShader("../models/room/roomShader.vs", "../models/room/roomShader.fs");

    // load models
    // -----------
    Model ourModel("../models/table/pooltable.obj");

    Model roomModel("../models/room/EmptyRoom(OBJ).obj");

    const glm::vec3 light_pos = glm::vec3(0.0, 3.0, 0.0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.76f, 0.88f, 1.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();
        roomShader.use();


        // light properties
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        ourShader.setVector3f("light.ambient", ambientColor * 10.0f);
        ourShader.setVector3f("light.diffuse", diffuseColor * 10.0f);
        ourShader.setVector3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        ourShader.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
        ourShader.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        ourShader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("material.shininess", 32.0f);


        roomShader.setVector3f("light.ambient", ambientColor * 10.0f);
        roomShader.setVector3f("light.diffuse", diffuseColor * 10.0f);
        roomShader.setVector3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        roomShader.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
        roomShader.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        roomShader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        roomShader.setFloat("material.shininess", 32.0f);



        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMatrix4("projection", projection);
        ourShader.setMatrix4("view", view);

        roomShader.setMatrix4("projection", projection);
        roomShader.setMatrix4("view", view);


        ourShader.setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVector3f("light.position", lightPos);
        ourShader.setVector3f("viewPos", camera.Position);

        roomShader.setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
        roomShader.setVector3f("light.position", lightPos);
        roomShader.setVector3f("viewPos", camera.Position);


        // render the loaded model
        glm::mat4 pooltable = glm::mat4(1.0f);
        pooltable = glm::translate(pooltable, glm::vec3(0.0f, 0.0f, 0.0f)); // position in the scene
        pooltable = glm::scale(pooltable, glm::vec3(10.0f, 10.0f, 10.0f));     // scale
        ourShader.setMatrix4("model", pooltable);
        ourModel.Draw(ourShader);

        glm::mat4 room = glm::mat4(1.0f);
        room = glm::translate(room, glm::vec3(0.0f, 0.0f, 0.0f)); // position in the scene
        room = glm::scale(room, glm::vec3(15.0f, 15.0f, 15.0f));     // scale
        roomShader.setMatrix4("model", room);
        roomModel.Draw(roomShader);


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

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
