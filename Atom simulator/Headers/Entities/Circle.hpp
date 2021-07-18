#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Circle
{
private:
    std::vector<float> vertices;
    //std::vector<unsigned int> indices;

    unsigned int segments;

    unsigned int VAO, VBO, EBO;

public:
    Circle(float xPos, float yPos, float zPos, unsigned int segments);

    Circle(Circle&) = delete;
    Circle(const Circle&) = delete;

    void bindCircle(unsigned int VAO, unsigned int VBO, unsigned int usage = GL_STATIC_DRAW);

    void setOffset(float xOffset, float yOffset, float zOffset);
    void setScale(float xScale, float yScale, float zScale);

    void draw();
};