#include "TestBasicTexture.hpp"
#include "Debugger.hpp"
#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>


namespace test {
	TestBasicTexture::TestBasicTexture() : m_Color {0.0f, 0.0f, 0.0f, 1.0f}{
		width  = 200;
		height = 200;
		float g_vertex_buffer_data[] = { 
			// 2D position, 	Relative Texture coordinate
			0.0,	0.0,		0.0f, 0.0f,
			width, 0.0,			1.0f, 0.0f,
			width, height,		1.0f, 1.0f,
			0.0,	height,	 	0.0f, 1.0f
		};

		// for ibo
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexBuffer* vb = new VertexBuffer(g_vertex_buffer_data, 4 * 4 * sizeof(float));
		VertexBufferLayout* layout = new VertexBufferLayout;
		layout->Push<float>(2);	// push position coordinate buffer
		layout->Push<float>(2);	// push texture coordinate buffer
		
		va = new VertexArray();
		va->AddBuffer(vb, layout);

		ib = new IndexBuffer(indices, 6);

		// doing glortho but via shader
		// Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
		proj = glm::ortho(	 0.0f,		// leftbound
							 800.0f, 	// rightbound
							 0.0f,		// bottombound
							 800.0f, 	// topbound
							-1.0f, 		// frontbound
							 1.0f);		// backbound
		// defines position and orientation of the "camera"
		//											camera coords
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		// defines position, rotation and scale of the vertices of the model in the world.
		//														 	model coords
		//glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
		// 		 move the model in the projection view.
		//		 move the projection by multiplying it with the view matrix
		// NOTE: MULTIPLY IN THIS ORDER OR IT BREAKS!

		// Create and compile our GLSL program from the shaders
		shader = new Shader("res/shaders/SimpleShader.glsl");
		shader->Bind();
		shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 0.5f);

		texture = new Texture("res/textures/gunsalpha.png");
		texture->Bind(0);	// bind slot should match u_Texture slot
		
		shader->SetUniform1i("u_Texture", 0);
		shader->SetUniform1i("u_Use_Texture", 1);	// enable textures


		va->UnBind();
		shader->UnBind();
		vb->UnBind();
		ib->UnBind();

		translation1 = glm::vec3(200, 200, 0);
		translation2 = glm::vec3(500, 200, 0);
	}
	TestBasicTexture::~TestBasicTexture() {}

	void TestBasicTexture::onUpdate(float deltaTime){}

	void TestBasicTexture::onRender(){
		model = glm::translate(glm::mat4(1.0f), translation1);

		// Use our shader
		shader->Bind();
		

		mvp = proj * view * model;
		shader->SetuniformMat4f("u_MVP", mvp);
		renderer->Draw(va, ib, shader);

		model = glm::translate(glm::mat4(1.0f), translation2);

		mvp = proj * view * model;
		shader->SetuniformMat4f("u_MVP", mvp);
		renderer->Draw(va, ib, shader);
		

		// change color
		if(m_Color[0] > 1.0){
			increment *= -1;
		}
		else if ( m_Color[0] < 0.0){
			increment *= -1;
		}

		m_Color[0] += increment;
	}

	void TestBasicTexture::onImGuiRender(){
		// draw ImGui window
		ImGui::SliderFloat3("Translation1", &translation1.x, 0.0f, 800.0f);
		ImGui::SliderFloat3("Translation2", &translation2.x, 0.0f, 800.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
