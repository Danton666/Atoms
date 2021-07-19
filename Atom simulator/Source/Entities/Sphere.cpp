#define _USE_MATH_DEFINES

#include "extern.hpp"

#include "Entities/Sphere.hpp"
#include "Tools/generator.hpp"

#include <cmath>

Sphere::Sphere(float xSize, float ySize, float zSize, unsigned int xS, unsigned int yS)
{
	++receptacle.vaoCounter;
	++receptacle.vboCounter;
	++receptacle.eboCounter;

	vertexNumber = receptacle.vaoCounter;
	elementaryNumber = receptacle.eboCounter;

	VAO = -1;
	VBO = -1;
	EBO = -1;

	xSegments = xS;
	ySegments = yS;

	countSegs = xS * yS * 6;

	for (unsigned int y = 0; y <= ySegments; ++y)
	{
		for (unsigned int x = 0; x <= xSegments; ++x)
		{
			float xSeg = (float)x / (float)xSegments;
			float ySeg = (float)y / (float)ySegments;

			float xPos = (float) (cos(xSeg * 2.f * M_PI) * sin(ySeg * M_PI) * xSize);
			float yPos =(float) (cos(ySeg * M_PI) * ySize);
			float zPos = (float) (sin(xSeg * 2.f * M_PI) * sin(ySeg * M_PI) * zSize);

			vertices.push_back(xPos);
			vertices.push_back(yPos);
			vertices.push_back(zPos);
		}
	}

	for (unsigned int y = 0; y < ySegments; ++y)
	{
		for (unsigned int x = 0; x < xSegments; ++x)
		{
			indices.push_back(y * (xSegments + 1) + x);
			indices.push_back((y + 1) * (xSegments + 1) + x);
			indices.push_back((y + 1) * (xSegments + 1) + (x + 1));
			indices.push_back(y * (xSegments + 1) + x);
			indices.push_back((y + 1) * (xSegments + 1) + (x + 1));
			indices.push_back(y * (xSegments + 1) + (x + 1));
		}
	}
}

void Sphere::setOffset(float xOffset, float yOffset, float zOffset)
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

void Sphere::setScale(float xScale, float yScale, float zScale)
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

void Sphere::bind(/*unsigned int VAO, unsigned int VBO, unsigned int EBO,*/ unsigned int usage)
{
	this->VAO = receptacle.VAO[vertexNumber - 1];
	this->VBO = receptacle.VBO[vertexNumber - 1];
	this->EBO = receptacle.EBO[elementaryNumber - 1];

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], usage);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sphere::draw()
{
	if (VAO != -1)
		glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, countSegs, GL_UNSIGNED_INT, 0);
}
