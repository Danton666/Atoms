#include "Tools\Camera.hpp"

void Camera::mouseControl()
{
    double xpos;
    double ypos;

    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    xoffset *= sensetivity;
    yoffset *= sensetivity;

    yaw     += xoffset;
    pitch   += yoffset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraTarget = glm::normalize(direction);
}

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) : cameraSpeed(1.f), window(nullptr)
{
    pitch = yaw = 0.f;
    lastX = lastY = 0.f;

    sensetivity = 1.f;

    firstMouse = true;

	cameraPos = pos;
	cameraTarget = target;
	cameraUp = up;
}

Camera::~Camera()
{
    window = nullptr;
    delete (void*)window;
}

void Camera::bind(GLFWwindow* window)
{
    this->window = window;
}

void Camera::setView(glm::mat4* view)
{
	(*view) = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);
}

void Camera::setSpeed	(float speed)					{ cameraSpeed = speed; }
void Camera::setPos		(const glm::vec3& pos)			{ cameraPos = pos; }
void Camera::setTarget	(const glm::vec3& target)		{ cameraTarget = target; }
void Camera::setUp		(const glm::vec3& up)			{ cameraUp = up; }

void Camera::setSensetivity(float sensetivity) { this->sensetivity = sensetivity; }

void Camera::control()
{
    if (window != nullptr)
    {
        mouseControl();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraTarget;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraTarget;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;
    }
}

void Camera::disableCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 1.5f, 1.5f);
}

