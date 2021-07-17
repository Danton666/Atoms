#define _USE_MATH_DEFINES

#include "Entities/Circle.hpp"

#include <iostream>
#include <cmath>

Circle::Circle(float xPos, float yPos, float zPos, unsigned int segments)
{
	VAO = 0;
	VBO = 0;
	EBO = 0;

	this->segments = segments;

	float angle = 360.f / (float)segments;

	//std::cout << "angle = " << angle << '\n';

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

void Circle::bindCircle(unsigned int VAO, unsigned int VBO, unsigned int usage)
{
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
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
	glLineWidth(2.f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, segments);
}
