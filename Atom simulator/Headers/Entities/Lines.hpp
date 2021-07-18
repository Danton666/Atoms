#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Lines
{
private:
    std::vector<float> vertices;

    unsigned int VAO, VBO;

    float width;

public:
    Lines(float x1, float y1, float z1, float x2, float y2, float z2);
    Lines(std::vector<float> vertices);

    Lines(Lines&) = delete;
    Lines(const Lines&) = delete;

    void bind(unsigned int VAO, unsigned int VBO, unsigned int usage = GL_STATIC_DRAW);

    void setWidth(float width);

    void draw();
};