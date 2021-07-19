#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Circle
{
private:
    unsigned int vertexNumber;
    unsigned int segments;
    unsigned int VAO, VBO;

    float width;

    std::vector<float> vertices;

public:
    Circle(float xPos, float yPos, float zPos, unsigned int segments);

    Circle(Circle&) = delete;
    Circle(const Circle&) = delete;

    void bind(unsigned int usage = GL_STATIC_DRAW);

    void setWidth(float width);
    void setOffset(float xOffset, float yOffset, float zOffset);
    void setScale(float xScale, float yScale, float zScale);

    void draw();
};