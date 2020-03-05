#include "TestGAS.hpp"

#include "VertexBufferStruct.hpp"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace test
{
TestGAS::TestGAS() : m_Color{0.0f, 0.0f, 0.0f, 1.0f}
{
	width = 1.0f;
	height = 1.0f;

	
	DrawBuffer<float> *VBDA = new DrawBuffer<float>({
		// 3D position, 			Texture coord		Color coord
		 0.0f,   0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
		width, 	 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		width, height, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
		 0.0f, height, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f, 1.0f}, 

		// IBO
		{0, 1, 2,
		 2, 3, 0}
	);
	/*
	DrawBuffer<float> *VBDA = new DrawBuffer<float>(
		// VBO
		{// 2D position, 	Texture coord		Color coord
		 0.0f,   0.0f, 		0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, 1.0f,
		width, 	 0.0f, 		1.0f, 0.0f,			0.0f, 1.0f, 0.0f, 1.0f,
		width, height,		1.0f, 1.0f, 		0.0f, 0.0f, 1.0f, 1.0f,
		 0.0f, height,		0.0f, 1.0f, 		1.0f, 1.0f, 1.0f, 1.0f}, 

		// IBO
		{0, 1, 2,
		 2, 3, 0},
		// layout
		{2, 2, 4}
	);
	*/
	shader = new Shader("res/shaders/SimpleShader.glsl");
	GAShape = new GenericAbstractShape<float>(shader, VBDA);
	

	Camera[0] = 0.5f;
	Camera[1] = 0.5f;
	Camera[2] = 2.0f;
	CameraLookat[0] = 0.5f;
	CameraLookat[1] = 0.5f;
	CameraLookat[2] = 0.0f;

	Perspective[0] = 90.0f;
	Perspective[1] = 1.0f;
	Perspective[2] = 0.1f;
	Perspective[3] = 10.0f;

	// doing glortho but via shader
	// Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.
	orth = glm::ortho( 0.0f,   // leftbound
					   800.0f, // rightbound
					   0.0f,   // bottombound
					   800.0f, // topbound
					  -1.0f,   // frontbound
					   1.0f);  // backbound
	pers = glm::perspective(glm::radians(Perspective[0]), 	// FOV
							 Perspective[1],				// aspectratio
							 Perspective[2],				// Znear
							 Perspective[3]);				// Zfar
	// defines position and orientation of the "camera"
	//											camera coords
	view = glm::translate(glm::mat4(1.0f), glm::vec3(Camera[0], Camera[1], Camera[2]));
	// defines position, rotation and scale of the vertices of the model in the world.
	//														 	model coords
	//glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	// 		 move the model in the projection view.
	//		 move the projection by multiplying it with the view matrix
	// NOTE: MULTIPLY IN THIS ORDER OR IT BREAKS!

	// Create and compile our GLSL program from the shaders
	//shader = new Shader("res/shaders/SimpleShader.glsl");
	//shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	texture = new Texture("res/textures/gunsalpha.png");
	//GAShape->SetTexture(texture);

	scale = 1.5f;

	GAShape->SetScale(scale, scale, 1.0f);

	translation1 = glm::vec3(0, 0, 0);
	translation2 = glm::vec3(-0.5, -0.5, 0);
}
TestGAS::~TestGAS() {}

void TestGAS::onUpdate() {}

void TestGAS::onRender()
{	
	proj = pers;
	// defines position and orientation of the "camera"
	//	2D										camera coords
	// view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
	view = glm::lookAt(glm::vec3(Camera[0], Camera[1], Camera[2]), glm::vec3(CameraLookat[0], CameraLookat[1], CameraLookat[2]), glm::vec3(0,1,0));
	model = glm::translate(glm::mat4(1.0f), translation1);

	// Use our shader
	GAShape->SetScale(scale, scale, 1.0f);

	mvp = proj * view * model;
	GAShape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	GAShape->Draw();

	model = glm::translate(glm::mat4(1.0f), translation2);

	mvp = proj * view * model;
	GAShape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	GAShape->Draw();

	// change color
	if (m_Color[0] > 1.0)
	{
		increment *= -1;
	}
	else if (m_Color[0] < 0.0)
	{
		increment *= -1;
	}

	m_Color[0] += increment;
}

void TestGAS::onImGuiRender()
{
	// draw ImGui window
	ImGui::SliderFloat3("Translation1", &translation1.x, -1.0f, 1.0f);
	ImGui::SliderFloat3("Translation2", &translation2.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Scale", &scale, 0.0f, 10.0f);
	ImGui::SliderFloat3("Camera", &Camera[0], -5.0f, 5.0f);
	ImGui::SliderFloat3("Lookat", &CameraLookat[0], -5.0f, 5.0f);
	//ImGui::SliderFloat("Perspective FOV", &Perspective[0], 0.0f, 180.0f);
	//ImGui::SliderFloat("Perspective ASPECT", &Perspective[1], 0.0f, 5.0f);
	//ImGui::SliderFloat2("Perspective Z", &Perspective[2], 0.0f, 10.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
} // namespace test
