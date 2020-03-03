#include <GL/glew.h>		// sudo apt-get install libglew-dev
#include <GLFW/glfw3.h>		// sudo apt-get install libglfw3-dev
#include <GL/gl.h>			
#include <glm/glm.hpp>		// sudo apt-get install libglm-dev

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.hpp"
#include "Renderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include "Texture.hpp"

void render(void);
void update(void);
void setVAOVBOIBO(void);
void setMVP(void);

GLFWwindow* window;

int main(){

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
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
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	// enable alpha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	float width  = 200;
	float height = 200;
	float g_vertex_buffer_data[] = { 
		// 2D position, 	Relative Texture coordinate
		 0.0,	0.0,		0.0f, 0.0f,
		 width, 0.0,		1.0f, 0.0f,
		 width, height,		1.0f, 1.0f,
		 0.0,	height,	 	0.0f, 1.0f
	};

	// for ibo
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexBuffer vb(g_vertex_buffer_data, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);	// push position coordinate buffer
	layout.Push<float>(2);	// push texture coordinate buffer
	
	VertexArray va;
	va.AddBuffer(&vb, &layout);

	IndexBuffer ib(indices, 6);

	// doing glortho but via shader
	// Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
	glm::mat4 proj = glm::ortho( 0.0f,				// leftbound
								 800.0f, 	// rightbound
								 0.0f,		// bottombound
								 800.0f, 	// topbound
								-1.0f, 		// frontbound
								 1.0f);		// backbound
	// defines position and orientation of the "camera"
	//														 camera coords
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	// defines position, rotation and scale of the vertices of the model in the world.
	//														 model coords
	glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(500, 200, 0));
	// 		 move the model in the projection view.
	//		 move the projection by multiplying it with the view matrix
	// NOTE: MULTIPLY IN THIS ORDER OR IT BREAKS!

	// Create and compile our GLSL program from the shaders
	Shader shader("res/shaders/SimpleShader.glsl");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 0.5f);
	

	Renderer renderer;

	Texture texture("res/textures/gunsalpha.png");
	texture.Bind(0);	// bind slot should match u_Texture slot
	
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniform1i("u_Use_Texture", 1);	// enable textures


	va.UnBind();
	shader.UnBind();
	vb.UnBind();
	ib.UnBind();

	float r = 0.0;
	float increment = 0.05;

	glm::mat4 mvp;
	do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		renderer.Clear();
		
		if(r > 1.0){
			increment = -0.05;
		}
		else if ( r < 0.0){
			increment = 0.05;
		}

		r += increment;

		// Use our shader
		shader.Bind();

		mvp = proj * view * model1;
		shader.SetuniformMat4f("u_MVP", mvp);
		renderer.Draw(&va, &ib, &shader);

		mvp = proj * view * model2;
		shader.SetuniformMat4f("u_MVP", mvp);
		renderer.Draw(&va, &ib, &shader);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}