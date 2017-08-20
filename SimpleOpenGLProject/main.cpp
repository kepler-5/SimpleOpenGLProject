#include "Classes/Camera.hpp"
#include "Classes/DeltaTimeGetter.hpp"
#include "Classes/Image.hpp"
#include "Classes/Shader.hpp"
#include "Classes/Types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {
	static const int windowWidth = 800;
	static const int windowHeight = 600;
	
	GLFWwindow* window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
		glfwSetWindowShouldClose(window, true);
}

int windowSetup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if ( !window ) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	
	// fear not scary error logs from GLFW:
	// https://github.com/glfw/glfw/issues/908
	// nothing super bad actually happens.

	return 0;
}

int gladSetup() {
	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	return 0;
}

void renderLoop() {
	// rendering setup?
	// ...
	while ( !glfwWindowShouldClose(window) ) {
		processInput(window);
		
		glClearColor(0.f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// camera.update(); // if you have a camera
		
		// rendering commands here
		// ...
		
		
		// poll events, swap buffer to the screen
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

#define INIT(f) do { if ( int error = f() ) return error; } while ( false )

int main() {
	INIT(windowSetup);
	INIT(gladSetup);
	
	renderLoop();
	
	glfwTerminate();
	
	return 0;
}
