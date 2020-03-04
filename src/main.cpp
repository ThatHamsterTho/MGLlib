#include <iostream>

// custom classes
#include "Window.h"
#include "ImGuiHelper.h"

#include "tests/TestClearColor.hpp"
#include "tests/TestBasicTexture.hpp"
#include "tests/TestBasicCamera.hpp"

void render(void);
void update(void);

using namespace MGLlib;

Window* window;
ImGuiHelper* IGH;

test::Test* currentTest;
test::TestMenu* TestMenu;

int main(){

	window = new Window("Window name", 800, 800);
	window->WindowEnableFeatures(Window::F_AA | Window::F_ALPHA | Window::F_VSYNC | Window::F_3D);

	IGH = new ImGuiHelper(window);

// INIT END HERE

	currentTest = nullptr;
	TestMenu = new test::TestMenu(currentTest);
	currentTest = TestMenu;

	TestMenu->AddTest<test::TestClearColor>("Clear Color");
	TestMenu->AddTest<test::TestBasicTexture>("Basic Texture");
	TestMenu->AddTest<test::TestBasicCamera>("Basic Camera");
	
	window->SetCallback(render);

	window->Show();

	IGH->Exit();
	window->Exit();

	return 0;
}

void render(void){
	
	// NOTE: ImGui stuff here
	if (currentTest){
		IGH->newFrame();

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