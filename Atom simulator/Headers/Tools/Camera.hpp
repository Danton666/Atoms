#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
private:
	GLFWwindow* window;

	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;

	float cameraSpeed;
	float sensetivity;

	bool firstMouse;

	float lastX;
	float lastY;

	float yaw;
	float pitch;

private:
	void mouseControl();

public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~Camera();

	Camera(Camera&) = delete;
	Camera(const Camera&) = delete;

	void bind(GLFWwindow* window);

	void setView(glm::mat4* view);

	void setSpeed(float speed);
	void setPos(const glm::vec3& pos);
	void setTarget(const glm::vec3& target);
	void setUp(const glm::vec3& up);

	void setSensetivity(float sensetivity);

	void control();

	void disableCursor();

};