#include "Atom.hpp"
#include <iostream>

int Atom::makeProgram(unsigned int* program, const char* vertexShader, const char* fragmentShader)
{
    unsigned int vSh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vSh, 1, &vertexShader, NULL);
    glCompileShader(vSh);
    if (Atom::checkErrors(vSh, "compile") != 0)
        return -1;

    unsigned int fSh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fSh, 1, &fragmentShader, NULL);
    glCompileShader(fSh);
    if (Atom::checkErrors(fSh, "compile") != 0)
        return -1;

    *program = glCreateProgram();
    glAttachShader(*program, vSh);
    glAttachShader(*program, fSh);
    glLinkProgram(*program);
    if (Atom::checkErrors(*program, "link") != 0)
        return -2;

    glDeleteShader(vSh);
    glDeleteShader(fSh);

    return 0;
}

int Atom::checkErrors(unsigned int entity, const char* type)
{
    int status;
    char log[512];

    if (std::string(type) == "compile")
    {
        glGetShaderiv(entity, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glGetShaderInfoLog(entity, 512, NULL, log);
            std::cout << "[COMPILE ERROR]: " << log;

            return -1;
        }
    }
    else if (std::string(type) == "link")
    {
        glGetProgramiv(entity, GL_LINK_STATUS, &status);
        if (!status)
        {
            glGetProgramInfoLog(entity, 512, NULL, log);
            std::cout << "[LINK ERROR]: " << log;

            return -2;
        }
    }
    else
    {
        std::cout << "[UNDEFINED ERROR TYPE]\n";

        return -3;
    }

    return 0;
}

float Atom::random(float min, float max)
{
    return (float)(rand()) / RAND_MAX * (max - min) + min;
}

Atom::Atom(float xPos, float yPos, float zPos)
{
    x_pos = xPos;
    y_pos = yPos;
    z_pos = zPos;

    core = new Sphere(0.1f, 0.1f, 0.1f, 15, 15);
    core->setOffset(xPos, yPos, zPos);

    electrons = new Sphere(0.07f, 0.07f, 0.07f, 15, 15);
    electrons->setOffset(xPos, yPos, zPos);

    circles = new Circle(0.f, 0.f, 0.f, 100);

    if (Atom::makeProgram(&shaderProgram, vertexShSrc, fragmentShSrc) != 0)
        this->~Atom();

    glUseProgram(shaderProgram);

    viewLoc = glGetUniformLocation(shaderProgram, "view");
    translationLoc = glGetUniformLocation(shaderProgram, "translation");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    colorLoc = glGetUniformLocation(shaderProgram, "color");

    coreColor = glm::vec3(1.f, 1.f, 1.f);
    electronsColor = glm::vec3(0.f, 0.f, 1.f);
    circlesColor = glm::vec3(1.f, 0.f, 0.f);

    projection = glm::mat4(glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f));

    view = glm::mat4(1.f);
    translation = glm::mat4(1.f);
    model = glm::mat4(1.f);
}

Atom::~Atom()
{
    delete circles;
    delete electrons;

    delete core;
}

void Atom::bind()
{
    /*Bind the core*/
    core->bind(GL_DYNAMIC_DRAW);

    /*Bind Electrons & circles*/
    electrons->bind(GL_DYNAMIC_DRAW);

    circles->bind(GL_STATIC_DRAW);
}

void Atom::setCoreColor(const glm::vec3& rgb)
{
    coreColor = rgb;
}

void Atom::setElectronsColor(const glm::vec3& rgb)
{
    electronsColor = rgb;
}

void Atom::setCirclesColor(const glm::vec3& rgb)
{
    circlesColor = rgb;
}

void Atom::draw()
{
    model = glm::mat4(1.f);
    translation = glm::mat4(1.f);

    glUseProgram(shaderProgram);

    float timeValue = (float)glfwGetTime();

    float radius = 1.f;

    float speedEl = 5.f;
    float speedCore = sinf(timeValue * 0.5f) * 1.3f;

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(colorLoc, 1, glm::value_ptr(electronsColor));

    /*draw electrons*/
    float a = 5.f;
    float b = 5.f;
    float c = 0.1f;

    glm::vec3 point;
    point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
    point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
    point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

    translation = glm::mat4(1.f);
    translation = glm::translate(translation, glm::normalize(point));

    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));

    electrons->draw();

    a = -5.f;
    b = 5.f;
    c = 0.1f;

    point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
    point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
    point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

    translation = glm::mat4(1.f);
    translation = glm::translate(translation, glm::normalize(point));

    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));

    electrons->draw();

    a = 0.1f;
    b = 10.f;
    c = 0.1f;

    point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
    point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
    point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

    translation = glm::mat4(1.f);
    translation = glm::translate(translation, glm::normalize(point));

    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));

    electrons->draw();

    a = -10.f;
    b = 0.1f;
    c = 0.1f;

    point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
    point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
    point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

    translation = glm::mat4(1.f);
    translation = glm::translate(translation, glm::normalize(point));

    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));

    electrons->draw();
    /*draw electrons*/

    /*draw the core*/
    glUniform3fv(colorLoc, 1, glm::value_ptr(coreColor));

    float diffusion = 0.1f;

    for (int i = 0; i < 7; ++i)
    {
        translation = glm::mat4(1.f);
        translation = glm::translate(translation, glm::vec3(speedCore * Atom::random(-diffusion, diffusion), speedCore * Atom::random(-diffusion, diffusion), speedCore * Atom::random(-diffusion, diffusion)));
        glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));

        core->draw();
    }
    /*draw the core*/

    glUniform3fv(colorLoc, 1, glm::value_ptr(circlesColor));

    /*draw circles*/
    translation = glm::mat4(1.f);
    model = glm::mat4(1.f);
    translation = glm::translate(translation, glm::vec3(x_pos, y_pos, z_pos));
    model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.f, 1.f, 0.f));

    glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    circles->draw();

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 1.f, 0.f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    circles->draw();

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    circles->draw();

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    circles->draw();
    /*draw circles*/
}

void Atom::setView(const glm::mat4& view)
{
    this->view = view;
}

void Atom::setTranslation(const glm::mat4& trans)
{
    this->translation = trans;
}

void Atom::setProjection(const glm::mat4& proj)
{
    this->projection = proj;
}

void Atom::setModel(const glm::mat4& model)
{
    this->model = model;
}
