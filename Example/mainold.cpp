#include <GL/glew.h>		// sudo apt-get install libglew-dev
#include <GLFW/glfw3.h>		// sudo apt-get install libglfw3-dev
#include <GL/gl.h>			
#include <glm/glm.hpp>		// sudo apt-get install libglm-dev

#include <iostream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"	// TODO: check what is neede from example files

// custom classes
#include "Renderer.hpp"

#include "tests/TestClearColor.hpp"
#include "tests/TestBasicTexture.hpp"
#include "tests/TestBasicCamera.hpp"

void render(void);
void update(void);
void setVAOVBOIBO(void);
void setMVP(void);

GLFWwindow* window;

int main(){
// INIT START HERE
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	// GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 800, 800, "Triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);	// enable vsync

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// enable alpha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// ImGUI init
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

// INIT END HERE

	test::Test* currentTest = nullptr;
	test::TestMenu* TestMenu = new test::TestMenu(currentTest);
	currentTest = TestMenu;

	TestMenu->AddTest<test::TestClearColor>("Clear Color");
	TestMenu->AddTest<test::TestBasicTexture>("Basic Texture");
	TestMenu->AddTest<test::TestBasicCamera>("Basic Camera");
	
	// handles drawing
	Renderer renderer;
	// loop until user closes the window
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 ){
		
        glfwPollEvents();	
		renderer.Clear();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		// NOTE: ImGui stuff here
		if (currentTest){
			currentTest->onUpdate(0.0f);
			currentTest->onRender();
			ImGui::Begin("Test");
				// back button
				if(currentTest != TestMenu && ImGui::Button("<-")){
					delete currentTest;
					currentTest = TestMenu;
				}
				currentTest->onImGuiRender();
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

	} // Check if the ESC key was pressed or the window was closed
	

	// close ImGui
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}