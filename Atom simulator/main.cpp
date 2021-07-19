#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "global.hpp"
#include "Tools/Camera.hpp"
#include "Tools/generator.hpp"

#include "Entities/Sphere.hpp"
#include "Entities/Circle.hpp"
#include "Entities/Lines.hpp"

#include "Atom.hpp"

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

    std::vector<Atom*> atoms;

    /*
    * Firstly need to create all
    * objects which use entities
    */
    for (int i = 0; i < 20; ++i)
        atoms.push_back(new Atom(Atom::random(-10.f, 10.f), Atom::random(-10.f, 10.f), Atom::random(-10.f, 10.f)));

    /*
    * Then call generateObjects()
    * to generate VAO, VBO, EBO
    */
    generateObjects();

    /*Lastly, call bind()*/
    for(int i = 0; i < atoms.size(); ++i)
        atoms[i]->bind();

    glm::mat4 view;

    Camera camera;
    camera.setPos(glm::vec3(0.0f, 0.0f, 5.f));
    camera.setSpeed(0.005f);

    /*
    * The camera doesn't use entities,
    * so it doesn't need VAO, VBO & EBO
    */
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

        for (int i = 0; i < atoms.size(); ++i)
        {
            atoms[i]->setView(view);
            atoms[i]->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < atoms.size(); ++i)
        delete atoms[i];

    /*
    * Delete VAO, VBO, EBO
    */
    cleanUpObjects();

    glfwTerminate();
    return 0;
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