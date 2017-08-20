#include "Camera.hpp"

namespace {
	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;
	
	template<typename T>
	const T& clamp(const T& value, const T& min, const T& max) {
		return std::min(std::max(value, min), max);
	}
}

#define GUARD_REGISTERED_CAMERA() do { if ( !Camera::registeredCamera ) return; } while ( false )

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	GUARD_REGISTERED_CAMERA();
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	GUARD_REGISTERED_CAMERA();
	Camera* camera = Camera::registeredCamera;
	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
	
	lastX = xpos;
	lastY = ypos;
	
	camera->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	GUARD_REGISTERED_CAMERA();
	Camera* camera = Camera::registeredCamera;
	camera->processMouseScroll(yoffset);
}

Camera* Camera::registeredCamera;

Camera::Camera(RegisterInfo info_,
			   glm::vec3 position,
			   glm::vec3 up,
			   float yaw,
			   float pitch)
: position(position)
, front(glm::vec3(0.0f, 0.0f, -1.0f))
, worldUp(up)
, projection(glm::perspective(glm::radians(45.0f), 800 / 600.f, 0.1f, 100.0f))
, yaw(yaw)
, pitch(pitch)
, movementSpeed(CameraDefaults::SPEED)
, mouseSensitivity(CameraDefaults::SENSITIVTY)
, zoom(CameraDefaults::ZOOM)
{
	updateCameraVectors();
	if ( auto window = info_.window )
		registerCamera(info_);
}

Camera::Camera(float posX,
			   float posY,
			   float posZ,
			   float upX,
			   float upY,
			   float upZ,
			   float yaw,
			   float pitch,
			   RegisterInfo info)
: Camera(info,
		 glm::vec3{posX, posY, posZ},
		 glm::vec3{upX, upY, upZ},
		 yaw,
		 pitch)
{}

Camera::~Camera() {
	if ( this == Camera::registeredCamera )
		Camera::registeredCamera = nullptr;
}

void Camera::registerCamera(RegisterInfo info_) {
	this->info = std::move(info_);
	auto window = this->info.window;
	projection = glm::perspective(glm::radians(45.0f), info_.size.w / info_.size.h, 0.1f, 100.0f);
	Camera::registeredCamera = this;
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

glm::mat4 Camera::getViewProjectionMatrix() const {
	return projection * glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (direction == Movement::FORWARD)
		position += front * velocity;
	if (direction == Movement::BACKWARD)
		position -= front * velocity;
	if (direction == Movement::LEFT)
		position -= right * velocity;
	if (direction == Movement::RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	
	yaw   += xoffset;
	pitch += yoffset;
	
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		pitch = clamp(pitch, -89.f, 89.f);
	}
	
	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	{
		// calculate the new front vector
		glm::vec3 newFront;
		newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
		newFront.y = std::sin(glm::radians(pitch));
		newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
		this->front = glm::normalize(newFront);
	}
	// also re-calculate the right and up vector
	right = glm::normalize(glm::cross(this->front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up    = glm::normalize(glm::cross(right, this->front));
}

void Camera::update() {
	GUARD_REGISTERED_CAMERA();
	Camera* camera = Camera::registeredCamera;
	const auto deltaTime = camera->info.dt.getDeltaTime();
	if(keys[GLFW_KEY_W])
		camera->processKeyboard(Movement::FORWARD, deltaTime);
	if(keys[GLFW_KEY_S])
		camera->processKeyboard(Movement::BACKWARD, deltaTime);
	if(keys[GLFW_KEY_A])
		camera->processKeyboard(Movement::LEFT, deltaTime);
	if(keys[GLFW_KEY_D])
		camera->processKeyboard(Movement::RIGHT, deltaTime);
	updateCameraVectors();
}
