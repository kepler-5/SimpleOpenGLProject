#ifndef Camera_hpp
#define Camera_hpp

#include "Classes/DeltaTimeGetter.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace CameraDefaults {
	constexpr float YAW        = -90.0f;
	constexpr float PITCH      =  0.0f;
	constexpr float SPEED      =  2.5f;
	constexpr float SENSITIVTY =  0.2f;
	constexpr float ZOOM       =  45.0f;
}

struct ViewSize {
	GLfloat w, h;
};

/** Allows you to move through the 3D space with WASD+mouse
 
	Usage example:
 
 Camera camera{* camera setup *};
 Shader yourObjectShader = ...; // whatever setup your shader needs
 
 // ...
 // in render loop:
 {
	 // process input
	 camera.update();
	 
	 // render commands
	 yourObjectShader.setUniform("transform", camera.getViewProjectionMatrix() * yourObjectTransform);
 }
 */
class Camera {
public:
	enum class Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	
	struct RegisterInfo {
		ViewSize size;
		GLFWwindow* window;
		DeltaTimeGetter dt;
		RegisterInfo(ViewSize s = {800.f, 600.f},
					 GLFWwindow* w = nullptr,
					 DeltaTimeGetter d = DeltaTimeGetter::defaultGetter())
		: size(s)
		, window(w)
		, dt(std::move(d))
		{}
	};
	
	Camera(RegisterInfo info,
		   glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = CameraDefaults::YAW,
		   float pitch = CameraDefaults::PITCH);

	Camera(float posX,
		   float posY,
		   float posZ,
		   float upX,
		   float upY,
		   float upZ,
		   float yaw,
		   float pitch,
		   RegisterInfo info = {});
	
	~Camera();

	glm::mat4 getViewProjectionMatrix() const;

	void processKeyboard(Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);
	
	const glm::vec3& getPosition() const { return position; }
	const glm::vec3& getFront() const { return front; }
	
	void registerCamera(RegisterInfo info);
	void update();
	
protected:
	void updateCameraVectors();
	
protected:
	RegisterInfo info;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	
	glm::mat4 projection;
	
	float yaw;
	float pitch;
	
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	
	static Camera* registeredCamera;
	
private:
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif /* Camera_h */

