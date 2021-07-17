#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Entities/Circle.hpp"
#include "Entities/Sphere.hpp"

class Atom
{
private:
	/*
	* VAO & VBO
	* [0]	- core
	* [1-4]	- electrons
	* [5-8] - circles
	* 
	* EBO
	* [0]	- core
	* [1-4] - electrons
	*/
	unsigned int VAO[9], VBO[9], EBO[5];

	Sphere* electrons[4];
	Sphere* core;

	Circle* circles[4];

	const char* vertexShSrc =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"out vec3 thisColor;\n"
		"uniform mat4 translation;"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform vec3 color;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projection * view * translation * model * vec4(pos, 1.0);\n"
		"    thisColor = color;\n"
		"}\0";

	const char* fragmentShSrc =
		"#version 330 core\n"
		"in vec3 thisColor;\n"
		"out vec4 outColor;\n"
		"void main()\n"
		"{\n"
		"    outColor = vec4(thisColor, 0.f);\n"
		"}\0";

	unsigned int shaderProgram;

	glm::vec3 coreColor;
	glm::vec3 electronsColor;
	glm::vec3 circlesColor;

	glm::mat4 view;
	glm::mat4 translation;
	glm::mat4 projection;
	glm::mat4 model;

	unsigned int viewLoc;
	unsigned int translationLoc;
	unsigned int projectionLoc;
	unsigned int modelLoc;

	unsigned int colorLoc;

private:
	static int makeProgram(unsigned int* program, const char* vertexShader, const char* fragmentShader);
	static int checkErrors(unsigned int entity, const char* type);

	static float random(float min, float max);

public:
	Atom(float xPos = 0.f, float yPos = 0.f, float zPos = 0.f);
	~Atom();

	Atom(Atom&) = delete;
	Atom(const Atom&) = delete;

	void bind(unsigned int VAO[9], unsigned int VBO[9], unsigned int EBO[5]);

	void setCoreColor(const glm::vec3& rgb);
	void setElectronsColor(const glm::vec3& rgb);
	void setCirclesColor(const glm::vec3& rgb);

	void setView(const glm::mat4& view);
	void setTranslation(const glm::mat4& trans);
	void setProjection(const glm::mat4& proj);
	void setModel(const glm::mat4& model);

	void draw();
};