#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Entities/Sphere.hpp"
#include "Entities/Circle.hpp"
#include "Entities/Lines.hpp"

#include "Tools/Camera.hpp"

#include <chrono>
#include <ctime>

#include <vector>

#include <iostream>

float random(float min, float max);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkErrors(unsigned int entity, const char* type);
void makeProgram(unsigned int* program, const char* vertexShader, const char* fragmentShader);

const char* vShSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"out vec3 myColor;\n"
"uniform mat4 trans;"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * trans * model * vec4(pos, 1.0);\n"
"    myColor = color;\n"
"}\0";

const char* fShSrc =
"#version 330 core\n"
"in vec3 myColor;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"    outColor = vec4(myColor, 0.f);\n"
"}\0";

const char* crVShSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(pos, 1.f);\n"
"}\0";

const char* crFShSrc =
"#version 330 core\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    color = vec3(0.f, 0.f, 1.f);\n"
"}\0";

const int Y_SEGM = 25;
const int X_SEGM = 25;

const float PI = 3.14159265358979323846f;

int main(int argc, char** argv)
{
	srand((unsigned int)time(0));
	rand();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Atom Simulator", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 800, 600);

	unsigned int shProg;
	makeProgram(&shProg, vShSrc, fShSrc);

	unsigned int crProg;
	makeProgram(&crProg, crVShSrc, crFShSrc);

	float coreSize = 0.2f;
	Sphere core(coreSize, coreSize, coreSize, 15, 15);

	float electronsSize = 0.07f;

	Sphere electron1(electronsSize, electronsSize, electronsSize, 15, 15);
	Sphere electron2(electronsSize, electronsSize, electronsSize, 15, 15);
	Sphere electron3(electronsSize, electronsSize, electronsSize, 15, 15);
	Sphere electron4(electronsSize, electronsSize, electronsSize, 15, 15);

	std::vector<float> linesVertices = 
	{
		-0.5f,  0.0f,  0.0f,
	  	 0.5f,  0.0f,  0.0f,

		 0.0f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f
	};
	Lines lines(linesVertices);

	unsigned int lnVAO, lnVBO;
	glGenVertexArrays(1, &lnVAO);
	glGenBuffers(1, &lnVBO);

	lines.bind(lnVAO, lnVBO, GL_STATIC_DRAW);

	Circle circle1(0.f, 0.f, 0.f, 100);
	Circle circle2(0.f, 0.f, 0.f, 100);
	Circle circle3(0.f, 0.f, 0.f, 100);
	Circle circle4(0.f, 0.f, 0.f, 100);
	unsigned int crVAO[4], crVBO[4];
	glGenVertexArrays(4, crVAO);
	glGenBuffers(4, crVBO);

	unsigned int VAO[5], VBO[5], EBO[5];
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(5, EBO);

	electron1.bindSphere(VAO[0], VBO[0], EBO[0], GL_DYNAMIC_DRAW);
	electron2.bindSphere(VAO[1], VBO[1], EBO[1], GL_DYNAMIC_DRAW);
	electron3.bindSphere(VAO[2], VBO[2], EBO[2], GL_DYNAMIC_DRAW);
	electron4.bindSphere(VAO[3], VBO[3], EBO[3], GL_DYNAMIC_DRAW);

	core.bindSphere(VAO[4], VBO[4], EBO[4], GL_DYNAMIC_DRAW);
	
	circle1.bindCircle(crVAO[0], crVBO[0], GL_STATIC_DRAW);
	circle2.bindCircle(crVAO[1], crVBO[1], GL_STATIC_DRAW);
	circle3.bindCircle(crVAO[2], crVBO[2], GL_STATIC_DRAW);
	circle4.bindCircle(crVAO[3], crVBO[3], GL_STATIC_DRAW);

	glUseProgram(shProg);

	unsigned int transLoc = glGetUniformLocation(shProg, "trans");
	unsigned int modelLoc = glGetUniformLocation(shProg, "model");
	unsigned int viewLoc = glGetUniformLocation(shProg, "view");
	unsigned int projectionLoc = glGetUniformLocation(shProg, "projection");

	unsigned int colorLoc = glGetUniformLocation(shProg, "color");

	glm::vec3 color;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::mat4 trans;

	Camera camera;
	camera.setPos(glm::vec3(0.0f, 0.0f, 5.f));
	camera.setSpeed(0.005f);

	camera.bind(window);
	camera.disableCursor();
	camera.setSensetivity(0.1f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		camera.control();

		float timeValue = (float)glfwGetTime();

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glUseProgram(crProg);

		glUseProgram(shProg);

		trans = glm::mat4(1.f);

		model = glm::mat4(1.f);
		view = glm::mat4(1.f);
		projection = glm::mat4(1.f);

		camera.setView(&view);
		float radius = 1.f;

		float speedEl = 5.f;
		float speedCore = sinf(timeValue * 0.5f) * 1.3f;

		float a = 5.f;
		float b = 5.f;
		float c = 0.1f;

		glm::vec3 point;
		point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
		point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
		point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

		trans = glm::translate(trans, glm::normalize(point));
		//model = glm::rotate(model, glm::radians(120.f), glm::vec3(0.f, 1.f, 1.f));

		color = glm::vec3(0.f, 0.f, 1.f);

		projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		electron1.draw();

		a = -5.f;
		b = 5.f;
		c = 0.1f;

		point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
		point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
		point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

		trans = glm::mat4(1.f);
		trans = glm::translate(trans, glm::normalize(point));

		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		electron2.draw();

		a = 0.1f;
		b = 10.f;
		c = 0.1f;

		point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
		point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
		point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

		trans = glm::mat4(1.f);
		trans = glm::translate(trans, glm::normalize(point));

		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		electron3.draw();

		a = -10.f;
		b = 0.1f;
		c = 0.1f;

		point.x = 0.0f + (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (c * cosf(timeValue * speedEl) - (a * b * sinf(timeValue * speedEl) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));
		point.y = 0.0f + (radius * sinf(timeValue * speedEl) * (sqrtf(powf(a, 2) + powf(c, 2))) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2)));
		point.z = 0.0f - (radius / sqrtf(powf(a, 2) + powf(c, 2))) * (a * cosf(timeValue * speedEl) + ((b * c * sinf(timeValue * speedEl)) / sqrtf(powf(a, 2) + powf(b, 2) + powf(c, 2))));

		trans = glm::mat4(1.f);
		trans = glm::translate(trans, glm::normalize(point));

		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		electron4.draw();

		color = glm::vec3(1.f, 1.f, 1.f);
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		float diffusion = 0.1f;

		for (int i = 0; i < 7; ++i)
		{
			trans = glm::mat4(1.f);
			trans = glm::translate(trans, glm::vec3(speedCore * random(-diffusion, diffusion), speedCore * random(-diffusion, diffusion), speedCore * random(-diffusion, diffusion)));
			glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

			core.draw();
		}

		/*draw circles*/
		trans = glm::mat4(1.f);
		color = glm::vec3(1.f, 0.f, 0.f);
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		circle1.draw();

		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		circle2.draw();

		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		circle3.draw();

		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		circle4.draw();
		/*draw circles*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(5, EBO);

	glfwTerminate();
	return 0;
}

float random(float min, float max)
{
	return (float)(rand()) / RAND_MAX * (max - min) + min;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void checkErrors(unsigned int entity, const char* type)
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
		}
	}
	else if (std::string(type) == "link")
	{
		glGetProgramiv(entity, GL_LINK_STATUS, &status);
		if (!status)
		{
			glGetProgramInfoLog(entity, 512, NULL, log);
			std::cout << "[LINK ERROR]: " << log;
		}
	}
	else
		std::cout << "[UNDEFINED ERROR TYPE]\n";
}

void makeProgram(unsigned int* program, const char* vertexShader, const char* fragmentShader)
{
	unsigned int vSh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vSh, 1, &vertexShader, NULL);
	glCompileShader(vSh);
	checkErrors(vSh, "compile");

	unsigned int fSh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fSh, 1, &fragmentShader, NULL);
	glCompileShader(fSh);
	checkErrors(fSh, "compile");

	*program = glCreateProgram();
	glAttachShader(*program, vSh);
	glAttachShader(*program, fSh);
	glLinkProgram(*program);
	checkErrors(*program, "link");

	glDeleteShader(vSh);
	glDeleteShader(fSh);
}