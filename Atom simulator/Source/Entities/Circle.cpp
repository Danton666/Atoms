#define _USE_MATH_DEFINES
#include "extern.hpp"

#include "Entities/Circle.hpp"
#include "Tools/generator.hpp"

#include <cmath>


Circle::Circle(float xPos, float yPos, float zPos, unsigned int segments)
{
    ++receptacle.vaoCounter;
    ++receptacle.vboCounter;

    vertexNumber = receptacle.vaoCounter;

    VAO = -1;
    VBO = -1;

    this->width = 1.f;

    this->segments = segments;

    for (unsigned int i = 0; i < segments; ++i)
    {
        float alpha = 2.f * (float)M_PI * float(i) / float(segments);

        float _xPos = cosf(alpha) + xPos;
        float _yPos = sinf(alpha) + yPos;
        float _zPos = zPos;

        vertices.push_back(_xPos);
        vertices.push_back(_yPos);
        vertices.push_back(_zPos);
    }
}

void Circle::bind(unsigned int usage)
{
    this->VAO = receptacle.VAO[vertexNumber - 1];
    this->VBO = receptacle.VBO[vertexNumber - 1];

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Circle::setWidth(float width)
{
    this->width = width;
}

void Circle::setOffset(float xOffset, float yOffset, float zOffset)
{
    auto xIt = vertices.begin();
    auto yIt = vertices.begin() + 1;
    auto zIt = vertices.begin() + 2;

    while (1)
    {
        (*xIt) += xOffset;
        (*yIt) += yOffset;
        (*zIt) += zOffset;

        xIt += 3;
        if (xIt == vertices.end())
            break;
        yIt += 3;
        zIt += 3;
    }
}

void Circle::setScale(float xScale, float yScale, float zScale)
{
    auto xIt = vertices.begin();
    auto yIt = vertices.begin() + 1;
    auto zIt = vertices.begin() + 2;

    while (1)
    {
        (*xIt) *= xScale;
        (*yIt) *= yScale;
        (*zIt) *= zScale;

        xIt += 3;
        if (xIt == vertices.end())
            break;
        yIt += 3;
        zIt += 3;
    }
}

void Circle::draw()
{
    glLineWidth(width);

    if(this->VAO != -1) 
        glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINE_LOOP, 0, segments);
}
