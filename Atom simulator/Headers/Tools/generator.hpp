#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

struct objects
{
	unsigned int vaoCounter;
	unsigned int vboCounter;
	unsigned int eboCounter;

	unsigned int* VAO;
	unsigned int* VBO;
	unsigned int* EBO;
};

void generateObjects();
void cleanUpObjects();

#endif //__GENERATOR_HPP__