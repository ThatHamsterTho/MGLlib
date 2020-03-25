#include "Core/3Drender/FPSCamera.h"

namespace MGLlib {
	FPSCamera::FPSCamera(Window* win){
		this->window = win;
		int width, height;
		glfwGetWindowSize(window->getGLFWwindow(), &width, &height);
		lastX = (float)width / 2.0f;
		lastY = (float)height / 2.0f;
	}

	FPSCamera::~FPSCamera(){}

	void FPSCamera::Move(GLFWwindow *glfwwindow){
		Camera* cam = window->GetCamera();
		const float cameraSpeed = 0.05f; // adjust accordingly
		if (glfwGetKey(glfwwindow, GLFW_KEY_W) == GLFW_PRESS)
			Pos += cameraSpeed * Front;
		if (glfwGetKey(glfwwindow, GLFW_KEY_S) == GLFW_PRESS)
			Pos -= cameraSpeed * Front;
		if (glfwGetKey(glfwwindow, GLFW_KEY_A) == GLFW_PRESS)
			Pos -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
		if (glfwGetKey(glfwwindow, GLFW_KEY_D) == GLFW_PRESS)
			Pos += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
		if (glfwGetKey(glfwwindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			Pos.y -= cameraSpeed;
		if (glfwGetKey(glfwwindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			Pos.y += cameraSpeed;
		cam->Position(Pos);
		cam->Lookat(Pos + Front);
		cam->SetUp(Up);
		cam->UpdateCamera();
	}

	void FPSCamera::Look(GLFWwindow *glfwwindow, double xpos, double ypos){
		if (glfwGetMouseButton(glfwwindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouse = true;
		}
		else {
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
				//glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (true)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			// Also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}
	}

	void FPSCamera::Scroll(GLFWwindow *glfwwindow, double xoffset, double yoffset){
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
		window->GetCamera()->SetFOV(Zoom);
		window->GetCamera()->UpdatePerspective();
	}
}