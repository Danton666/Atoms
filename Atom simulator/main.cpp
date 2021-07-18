#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Entities/Sphere.hpp"
#include "Entities/Circle.hpp"
#include "Entities/Lines.hpp"

#include "Atom.hpp"

#include "Tools/Camera.hpp"

#include <chrono>
#include <ctime>

#include <vector>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(int argc, char** argv)
{
    srand((unsigned int)time(0));
    rand();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Atom Simulator", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 800, 600);

    unsigned int VAO[18], VBO[18], EBO[10];
    glGenVertexArrays(18, VAO);
    glGenBuffers(18, VBO);
    glGenBuffers(10, EBO);

    unsigned int VAO1[9], VBO1[9], EBO1[5];
    unsigned int VAO2[9], VBO2[9], EBO2[5];
    for (int i = 0; i < 9; ++i)
    {
        VAO1[i] = VAO[i];
        VBO1[i] = VBO[i];

        VAO2[i] = VAO[i + 9];
        VBO2[i] = VBO[i + 9];

        if (i < 5)
        {
            EBO1[i] = EBO[i];
            EBO2[i] = EBO[i + 5];
        }
    }

    Atom atom(1.f, 1.f, 1.f);
    atom.bind(VAO1, VBO1, EBO1);

    memset(VAO1, 0, sizeof(VAO1));
    memset(VBO1, 0, sizeof(VBO1));
    memset(EBO1, 0, sizeof(EBO1));

    Atom atom2(0.5f, 5.f, 0.f);
    atom2.bind(VAO2, VBO2, EBO2);

    memset(VAO2, 0, sizeof(VAO2));
    memset(VBO2, 0, sizeof(VBO2));
    memset(EBO2, 0, sizeof(EBO2));

    glm::mat4 view;

    Camera camera;
    camera.setPos(glm::vec3(0.0f, 0.0f, 5.f));
    camera.setSpeed(0.005f);

    camera.bind(window);
    camera.disableCursor();
    camera.setSensetivity(0.1f);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        camera.control();

        float timeValue = (float)glfwGetTime();

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::mat4(1.f);

        camera.setView(&view);

        atom.setView(view);
        atom.draw();
        atom2.setView(view);
        atom2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
    glDeleteVertexArrays(18, VAO);
    glDeleteBuffers(18, VBO);
    glDeleteBuffers(10, EBO);


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}