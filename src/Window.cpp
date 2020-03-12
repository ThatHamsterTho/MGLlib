#include "Window.h"

#include <vector>

#include "Debugger.hpp"
#include "UtilFunc.hpp"


namespace MGLlib{

Window::Window(const char* title, int width, int height, int offsetx, int offsety) {
	
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( width, height, title, NULL, NULL);
	if( window == NULL ){
		glfwTerminate();
		pERROR("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, offsetx, offsety);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		pERROR("Failed to initialize GLEW");
		exit(-1);
	}

	#ifdef SHOWGLVERSION
		pINFO("OpenGL Version: %s", glGetString(GL_VERSION));
	#endif

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, width/2, height/2);
	// Dark blue background
	glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);

	InitHandler(window);
	//Enableortho();

	Util::quickrandomsetseed();
}
Window::~Window(){}

// Features
void Window::enableLighting(void){
	#ifdef SHOWINITINFO
		pINFO("Lighting init");
	#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}
void Window::enable3D(void){
	#ifdef SHOWINITINFO
		pINFO("3D init");
	#endif
	// Use depth buffering for hidden surface elimination.
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LEQUAL);
	Enable3Drender();
	//Disableortho();
}
void Window::enableAA(void){
	#ifdef SHOWINITINFO
		pINFO("Anti Aliasing init");
	#endif
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	glEnable(GL_MULTISAMPLE);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	GLint  iMultiSample = 0;
	GLint  iNumSamples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	glGetIntegerv(GL_SAMPLES, &iNumSamples);
	#ifdef SHOWMSAA
		pINFO("MultiSample: %u, Samples: %u", iMultiSample, iNumSamples);
	#endif 
}
void Window::enableAlpha(void){
	#ifdef SHOWINITINFO
		pINFO("Alpha init");
	#endif	
	// enable alpha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// dual depth peeling,,, hmmm interresting, but out of scope...
	// http://developer.download.nvidia.com/SDK/10/opengl/src/dual_depth_peeling/doc/DualDepthPeeling.pdf

	//GLCall(glBlendEquation(GL_FUNC_ADD)); 
	//GLCall(glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA)); 
}
void Window::enableVsync(void){
	#ifdef SHOWINITINFO
		pINFO("Vsync init");
	#endif	
	glfwSwapInterval(1);
}

void Window::EnableFeatures(unsigned int f){
	std::vector<WindowVoidFunc> featurefuncs;

	if(f & F_3D){
		featurefuncs.push_back(&Window::enable3D);
	}
	if(f & F_AA){
		featurefuncs.push_back(&Window::enableAA);
	}
	if(f & F_ALPHA){
		featurefuncs.push_back(&Window::enableAlpha);
	}
	if(f & F_LIGHTING){
		featurefuncs.push_back(&Window::enableLighting);
	}
	if(f & F_VSYNC){
		featurefuncs.push_back(&Window::enableVsync);
	}

	for(int i = 0; i < static_cast<int>(featurefuncs.size()); i++){
		// this needed because of context to the function
		(this->*featurefuncs[i])();
	}
}

void Window::SetCallback(void (*render)(void), void (*update)(void)){
	this->render = render;
	this->update = update;
}
void Window::Show(void){
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 ){
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			if(update){
				update();
			}
			if(render){
				render();
			}
			else{
				fprintf(stderr, "NO RENDER FUNCTION");
			}
		
		glfwSwapBuffers(window);	
	}
}

void Window::Exit(void){
	glfwDestroyWindow(window);
	glfwTerminate();
}

} // namespace
