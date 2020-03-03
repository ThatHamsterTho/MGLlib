#include <GL/glew.h>		// sudo apt-get install libglew-dev
#include <GL/freeglut.h>	// sudo apt-get install freeglut3-dev
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

VertexArray *va;
VertexBufferLayout *layout;
VertexBuffer *vb;
IndexBuffer *ib;
Shader *shader;
Renderer *renderer;

Texture *texture;


int main(int ac, char **ap){
	glutInit(&ac, ap);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(2000, 100);
	glutCreateWindow("triangle");

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		return -1;
	}

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	setVAOVBOIBO();	// for the square

	va->UnBind();
	shader->UnBind();
	vb->UnBind();
	ib->UnBind();

	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutMainLoop();

	return 0;
}


void setVAOVBOIBO(void){
	float g_vertex_buffer_data[] = { 
		// 2D position, 	Relative Texture coordinate
		100.0f, 100.0f,		0.0f, 0.0f,
		400.0f, 100.0f,		1.0f, 0.0f,
		400.0f, 400.0f,		1.0f, 1.0f,
		100.0f, 400.0f, 	0.0f, 1.0f
	};

	// for ibo
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// enable alpha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	

	va = new VertexArray();

	vb = new VertexBuffer(g_vertex_buffer_data, 4 * 4 * sizeof(float));
	layout = new VertexBufferLayout();
	layout->Push<float>(2);	// push position coordinate buffer
	layout->Push<float>(2);	// push texture coordinate buffer
	
	va->AddBuffer(vb, layout);

	ib = new IndexBuffer(indices, 6);

	// doing glortho but via shader
	// Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
	glm::mat4 proj = glm::ortho(0.0f, 								// leftbound
								(float)glutGet(GLUT_WINDOW_WIDTH), 	// rightbound
								0.0f, 								// bottombound
								(float)glutGet(GLUT_WINDOW_HEIGHT), // topbound
								-1.0f, 								// frontbound
								1.0f);								// backbound
	// defines position and orientation of the "camera"
	//														 camera coords
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	// defines position, rotation and scale of the vertices of the model in the world.
	//														 model coords
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	
	// 		 move the model in the projection view.
	//		 move the projection by multiplying it with the view matrix
	// NOTE: MULTIPLY IN THIS ORDER OR IT BREAKS!
	glm::mat4 mvp = proj * view * model;

	// Create and compile our GLSL program from the shaders
	// programID = LoadShaders("res/shaders/SimpleVertexShader.vertexshader", "res/shaders/SimpleFragmentShader.fragmentshader" );
	shader = new Shader("res/shaders/SimpleShader.Shader");
	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader->SetuniformMat4f("u_MVP", mvp);

	renderer = new Renderer();

	texture = new Texture("res/textures/gunsalpha.png");
	texture->Bind(0);	// bind slot should match u_Texture slot
	shader->SetUniform1i("u_Texture", 0);
}

float r = 0.0;
float increment = 0.05;

void render(void){
	renderer->Clear();

	// Use our shader
	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.0, 0.0, 0.0, 0.0);

	renderer->Draw(va, ib, shader);

	//glFlush();
	glutSwapBuffers();
}

void update(void){
	if(r > 1.0){
		increment = -0.05;
	}
	else if ( r < 0.0){
		increment = 0.05;
	}

	r += increment;

	glutPostRedisplay();
}