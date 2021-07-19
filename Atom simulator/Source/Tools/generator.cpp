#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Tools\generator.hpp"

extern struct objects receptacle;

void generateObjects()
{
	receptacle.VAO = new unsigned int[receptacle.vaoCounter];
	receptacle.VBO = new unsigned int[receptacle.vboCounter];
	receptacle.EBO = new unsigned int[receptacle.eboCounter];

	glGenVertexArrays(receptacle.vaoCounter, receptacle.VAO);
	glGenBuffers(receptacle.vboCounter, receptacle.VBO);
	glGenBuffers(receptacle.eboCounter, receptacle.EBO);
}

void cleanUpObjects()
{
	glDeleteVertexArrays(receptacle.vaoCounter, receptacle.VAO);
	glDeleteBuffers(receptacle.vboCounter, receptacle.VBO);
	glDeleteBuffers(receptacle.eboCounter, receptacle.EBO);
}
