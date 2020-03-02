#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

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
		// 2D position, Relative Texture coordinate
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
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

	// Create and compile our GLSL program from the shaders
	// programID = LoadShaders("res/shaders/SimpleVertexShader.vertexshader", "res/shaders/SimpleFragmentShader.fragmentshader" );
	shader = new Shader("res/shaders/SimpleShader.Shader");

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