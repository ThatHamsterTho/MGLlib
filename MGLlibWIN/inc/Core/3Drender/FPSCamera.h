#ifndef FPSCAMERAHEADERGUARD
#define FPSCAMERAHEADERGUARD

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/3Drender/Camera.h"
#include "Window.h"

namespace MGLlib {
	class FPSCamera {
		glm::vec3 Pos   = glm::vec3(0.0f, 0.0f,  3.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up 	= glm::vec3(0.0f, 1.0f, 0.0f);	
		glm::vec3 Right;
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		// Default camera values
		float lastX;
		float lastY;
		bool firstMouse = true;

		float Yaw         = -90.0f;
		float Pitch       =  0.0f;

		const float MouseSensitivity = 0.1f;

		float Zoom = 45.0f;

		Window* window;

		public:
			FPSCamera(Window* win);
			~FPSCamera();

			void Move(GLFWwindow *glfwwindow);
			void Look(GLFWwindow *glfwwindow, double xpos, double ypos);

			void Scroll(GLFWwindow *glfwwindow, double xoffset, double yoffset);

	};
}


#endif