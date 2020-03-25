#include <iostream>

#include "Core/3Drender/FPSCamera.h"
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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

using namespace MGLlib;

Window* window;
ImGuiHelper* IGH;

FPSCamera* FPScam;


test::Test* currentTest;
test::TestMenu* TestMenu;

int main(){

	window = new Window("Window name", SCR_WIDTH, SCR_HEIGHT);
	window->EnableFeatures(Window::F_AA | Window::F_ALPHA | Window::F_VSYNC | Window::F_3D);

	IGH = new ImGuiHelper(window);
	FPScam = new FPSCamera(window);

	//glfwSetKeyCallback(window->getGLFWwindow(), key_callback);
	//glfwSetCursorPosCallback(window->getGLFWwindow(), cursor_position_callback);
	glfwSetFramebufferSizeCallback(window->getGLFWwindow(), framebuffer_size_callback);

	glfwSetCursorPosCallback(window->getGLFWwindow(), mouse_callback);
    glfwSetScrollCallback(window->getGLFWwindow(), scroll_callback);

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
	
	// clean up of global instanses
	delete IGH;
	delete FPScam;
	if(TestMenu != currentTest){
		delete currentTest;
	}
	delete TestMenu;


	Window::Destroy(window);
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



void processInput(GLFWwindow *glfwwindow)
{
	FPScam->Move(glfwwindow);
}

void framebuffer_size_callback(GLFWwindow* glfwwindow, int width, int height)
{
    window->framebuffer_resize(glfwwindow, width, height);
}

void mouse_callback(GLFWwindow* glfwwindow, double xpos, double ypos)
{
	FPScam->Look(glfwwindow, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    FPScam->Scroll(window, xoffset, yoffset);
}