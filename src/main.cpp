#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <iostream>

#include "Shader.hpp"
#include "Renderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

void render(void);
void update(void);
void setVAOVBOIBO(void);

VertexArray *va;
VertexBufferLayout *layout;
VertexBuffer *vb;
IndexBuffer *ib;
Shader *shader;
Renderer *renderer;



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
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	// for ibo
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	va = new VertexArray();

	vb = new VertexBuffer(g_vertex_buffer_data, 4 * 3 * sizeof(float));
	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	
	va->AddBuffer(vb, layout);

	ib = new IndexBuffer(indices, 6);

	// Create and compile our GLSL program from the shaders
	// programID = LoadShaders("res/shaders/SimpleVertexShader.vertexshader", "res/shaders/SimpleFragmentShader.fragmentshader" );
	shader = new Shader("res/shaders/SimpleShader.Shader");

	renderer = new Renderer();
}

float r = 0.0;
float increment = 0.05;

void render(void){
	renderer->Clear();

	// Use our shader
	shader->Bind();
	shader->SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

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