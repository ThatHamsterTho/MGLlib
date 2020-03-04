#include "Window.h"

#include <vector>
#include <iostream>

#include "Debugger.hpp"

namespace MGLlib{
	Window::Window(const char* title, int width, int height, int offsetx, int offsety){
		
		// Initialise GLFW
		if( !glfwInit() )
		{
			throw "Failed to initialize GLFW\n";
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
			throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
			exit(-1);
		}
		glfwMakeContextCurrent(window);
		glfwSetWindowPos(window, offsetx, offsety);
		

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			throw "Failed to initialize GLEW\n";
			exit(-1);
		}

		#ifdef SHOWGLVERSION
			printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
		#endif

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Dark blue background
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	}
	Window::~Window(){}

	// Features
	void Window::enableLighting(void){
		printf("Lighting init\n");
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	}
	void Window::enable3D(void){
		printf("3D init\n");
		// Use depth buffering for hidden surface elimination.
		glEnable(GL_DEPTH_TEST);	
		glDepthFunc(GL_LEQUAL);
	}
	void Window::enableAA(void){
		printf("Anti Aliasing init\n");
		//glEnable(GL_LINE_SMOOTH);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//glEnable(GL_POINT_SMOOTH);
		//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		
		glEnable(GL_MULTISAMPLE);
		//glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

		GLint  iMultiSample = 0;
		GLint  iNumSamples = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
		glGetIntegerv(GL_SAMPLES, &iNumSamples);
		#ifdef SHOWMSAA
			printf("MultiSample: %u, Samples: %u\n", iMultiSample, iNumSamples);
		#endif 
	}
	void Window::enableAlpha(void){
		printf("Alpha init\n");
		// enable alpha
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	void Window::enableVsync(void){
		printf("Vsync init\n");
		glfwSwapInterval(1);
	}

	void Window::WindowEnableFeatures(unsigned int f){
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
		glfwTerminate();
	}
}

