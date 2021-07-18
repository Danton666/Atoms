#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Sphere
{
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int xSegments;
    unsigned int ySegments;

    unsigned int countSegs;

    unsigned int VAO, VBO, EBO;

public:
    Sphere(float xSize = 1.f, float ySize = 1.f, float zSize = 1.f, unsigned int xS = 25, unsigned int yS = 25);

    Sphere(Sphere&) = delete;
    Sphere(const Sphere&) = delete;

    void setOffset(float xOfffset, float yOffset, float zOffset);
    void setScale(float xScale, float yScale, float zScale);

    void bindSphere(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int usage = GL_STATIC_DRAW);

    void draw();
};