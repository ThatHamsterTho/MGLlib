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
void setMVP(void);

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

	// enable alpha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

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

glm::mat4 proj;
glm::mat4 view;
glm::mat4 model1;
glm::mat4 model2;
glm::mat4 mvp;

void setVAOVBOIBO(void){
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
	

	va = new VertexArray();

	vb = new VertexBuffer(g_vertex_buffer_data, 4 * 4 * sizeof(float));
	layout = new VertexBufferLayout();
	layout->Push<float>(2);	// push position coordinate buffer
	layout->Push<float>(2);	// push texture coordinate buffer
	
	va->AddBuffer(vb, layout);

	ib = new IndexBuffer(indices, 6);

	// doing glortho but via shader
	// Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
	proj = glm::ortho( 0.0f,									// leftbound
								 (float)glutGet(GLUT_WINDOW_WIDTH), 	// rightbound
								 0.0f,									// bottombound
								 (float)glutGet(GLUT_WINDOW_HEIGHT), 	// topbound
								-1.0f, 									// frontbound
								 1.0f);									// backbound
	// defines position and orientation of the "camera"
	//														 camera coords
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	// defines position, rotation and scale of the vertices of the model in the world.
	//														 model coords
	model1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	model2 = glm::translate(glm::mat4(1.0f), glm::vec3(500, 200, 0));
	// 		 move the model in the projection view.
	//		 move the projection by multiplying it with the view matrix
	// NOTE: MULTIPLY IN THIS ORDER OR IT BREAKS!

	// Create and compile our GLSL program from the shaders
	shader = new Shader("res/shaders/SimpleShader.glsl");
	shader->Bind();
	shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	

	renderer = new Renderer();

	texture = new Texture("res/textures/gunsalpha.png");
	texture->Bind(0);	// bind slot should match u_Texture slot
	shader->SetUniform1i("u_Texture", 0);
	shader->SetUniform1i("u_Use_Texture", 1);
}

float r = 0.0;
float increment = 0.05;

void render(void){
	renderer->Clear();

	// Use our shader
	shader->Bind();

	mvp = proj * view * model1;
	shader->SetuniformMat4f("u_MVP", mvp);
	renderer->Draw(va, ib, shader);

	mvp = proj * view * model2;
	shader->SetuniformMat4f("u_MVP", mvp);
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