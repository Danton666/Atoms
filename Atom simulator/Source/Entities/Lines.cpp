#include "extern.hpp"

#include "Entities\Lines.hpp"
#include "Tools/generator.hpp"

Lines::Lines(float x1, float y1, float z1, float x2, float y2, float z2)
{
    ++receptacle.vaoCounter;
    ++receptacle.vboCounter;

    vertexNumber = receptacle.vaoCounter;

    this->VAO = -1;
    this->VBO = -1;

    this->width = 0.f;

    vertices.push_back(x1);
    vertices.push_back(y1);
    vertices.push_back(z1);

    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(z2);
}

Lines::Lines(std::vector<float> vertices)
{
    this->VAO = 0;
    this->VBO = 0;

    this->width = 0.f;

    this->vertices = vertices;
}

void Lines::bind(unsigned int usage)
{
    this->VAO = receptacle.VAO[vertexNumber - 1];
    this->VBO = receptacle.VBO[vertexNumber - 1];

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Lines::setWidth(float width)
{
    this->width = width;
}

void Lines::draw()
{
    glLineWidth(width);
        if(VAO != -1)
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size() / 3);
}
