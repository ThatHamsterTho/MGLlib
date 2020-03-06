#include "TestGAS.hpp"

#include "DrawBuffer.hpp"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace MGLlib {
using namespace Primitives;
namespace test {
TestGAS::TestGAS(MGLlib::Window* window) : Test(window), m_Color{0.0f, 0.0f, 0.0f, 1.0f}
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

	cam = new Camera({1.0, 1.0, 3.5}, {1.0, 1.0, 0.0});
	cam->SetFOV(90);

	// defines position, rotation and scale of the vertices of the model in the world.
	//														 	model coords
	//glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	// Create and compile our GLSL program from the shaders
	//shader = new Shader("res/shaders/SimpleShader.glsl");
	//shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	texture = new Texture("res/textures/gunsalpha.png");
	//GAShape->SetTexture(texture);

	scale = 1.5f;

	GAShape->SetScale(scale, scale, 1.0f);

	translation1 = glm::vec3(0, 0, 0);
	translation2 = glm::vec3(-0.5, -0.5, 0);

	// free Vertexbufferdata
	delete VBDA;
}
TestGAS::~TestGAS() {
	delete cam;
	delete texture;
	delete GAShape;
	delete shader;
}

void TestGAS::onUpdate() {}

void TestGAS::onRender()
{	
	// defines position and orientation of the "camera"
	//	2D										camera coords
	// view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
	model = glm::translate(glm::mat4(1.0f), translation1);

	// Use our shader
	GAShape->SetScale(scale, scale, 1.0f);

	mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * model;
	GAShape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	GAShape->Draw();

	model = glm::translate(glm::mat4(1.0f), translation2);

	mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * model;
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
	ImGui::SliderFloat3("Camera", &cam->getPosition()[0], -5.0f, 5.0f);
	ImGui::SliderFloat3("Lookat", &cam->getLookat()[0], -5.0f, 5.0f);
	//ImGui::SliderFloat("Perspective FOV", &Perspective[0], 0.0f, 180.0f);
	//ImGui::SliderFloat("Perspective ASPECT", &Perspective[1], 0.0f, 5.0f);
	//ImGui::SliderFloat2("Perspective Z", &Perspective[2], 0.0f, 10.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	cam->UpdateCamera();
}
} // namespace test
} // namespace MGLlib