#include <iostream>

#include "Window.h"

// custom classes
#include "ImGuiHelper.h"
#include "tests/TestGAS.hpp"
#include "tests/TestBasicTexture.hpp"
#include "tests/TestShapeHandler.h"


void render(void);
void update(void);
void framebuffer_size_callback(GLFWwindow* glfwwindow, int width, int height);
void mouse_callback(GLFWwindow* glfwwindow, double xpos, double ypos);
void scroll_callback(GLFWwindow* glfwwindow, double xoffset, double yoffset);
void processInput(GLFWwindow *glfwwindow);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

using namespace MGLlib;

Window* window;
ImGuiHelper* IGH;

test::Test* currentTest;
test::TestMenu* TestMenu;

int main(){

	window = new Window("Window name", SCR_WIDTH, SCR_HEIGHT);
	window->EnableFeatures(Window::F_AA | Window::F_ALPHA | Window::F_VSYNC | Window::F_3D);

	//glfwSetKeyCallback(window->getGLFWwindow(), key_callback);
	//glfwSetCursorPosCallback(window->getGLFWwindow(), cursor_position_callback);
	glfwSetFramebufferSizeCallback(window->getGLFWwindow(), framebuffer_size_callback);

	glfwSetCursorPosCallback(window->getGLFWwindow(), mouse_callback);
    glfwSetScrollCallback(window->getGLFWwindow(), scroll_callback);

	IGH = new ImGuiHelper(window);

	// test stuff
	currentTest = nullptr;
	TestMenu = new test::TestMenu(currentTest, window);
	currentTest = TestMenu;

	TestMenu->AddTest<test::TestShapeHandler>("ShapeHandlerTest");
	TestMenu->AddTest<test::TestGAS>("GenericAbstractShape");
	TestMenu->AddTest<test::TestBasicTexture>("TextureTest");

	window->SetCallback(render);
	window->Show();

	IGH->Exit();
	window->Exit();

	// clean up of global instanses
	delete window;
	delete IGH;
	if(TestMenu != currentTest){
		delete currentTest;
	}
	delete TestMenu;

	return 0;
}

void render(void){
	
	// NOTE: ImGui stuff here
	if (currentTest){
		IGH->newFrame();
		
		processInput(window->getGLFWwindow());

		currentTest->onUpdate();
		currentTest->onRender();

		ImGui::Begin("Test");
			// back button
			if(currentTest != TestMenu && ImGui::Button("<-")){
				delete currentTest;
				currentTest = TestMenu;
			}
			currentTest->onImGuiRender();
		ImGui::End();

		IGH->render();
	}
}

glm::vec3 Pos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);	
glm::vec3 Right;
glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *glfwwindow)
{
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* glfwwindow, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
	window->GetCamera()->UpdateAspectRatio(width, height);
	window->GetCamera()->UpdatePerspective();
}


// Default camera values
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float Yaw         = -90.0f;
float Pitch       =  0.0f;

const float MouseSensitivity = 0.1f;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* glfwwindow, double xpos, double ypos)
{
	if(glfwGetMouseButton(glfwwindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
	else{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw   += xoffset;
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
		Up    = glm::normalize(glm::cross(Right, Front));
	}
}

float Zoom = 45.0f;

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}