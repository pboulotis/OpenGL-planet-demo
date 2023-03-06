// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <ctime>


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

// Initial camera_position : on +Z
glm::vec3 camera_position = glm::vec3(0, 0, 80);

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}
glm::vec3 getCameraPosition() {
	return camera_position;
}

// Sun's center
glm::vec3 center = glm::vec3(0, 0, 0);

glm::vec3 up_angle = glm::vec3(0, 1, 0);

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second



void computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 800 / 2, 800 / 2);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction = glm::normalize(center - camera_position);

	// Right vector
	glm::vec3 right = glm::normalize(glm::cross(up_angle, direction));

	// Up vector
	glm::vec3 up = glm::normalize(glm::cross(right, direction));

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		camera_position += (center - camera_position) * deltaTime * 0.3f;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		camera_position -= (center - camera_position) * deltaTime * 0.3f;
	}
	// Strafe up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_position += up * deltaTime * speed * 10.0f;
	}
	// Strafe down
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		camera_position -= up * deltaTime * speed * 10.0f;
	}

	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_position += right * deltaTime * speed * 10.0f;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_position -= right * deltaTime * speed * 10.0f;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
	
	// Projection matrix : 45° Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f / 1.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		camera_position,           // Camera is here
		center,			// and looks here : at the same camera_position, plus "direction"
		up_angle	// Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}