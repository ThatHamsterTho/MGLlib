#include "tests/TestGAS.hpp"

#include "ImGui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"


namespace MGLlib {
using namespace Primitives;
namespace test {
TestGAS::TestGAS(Window* window) : Test(window), m_Color{0.0f, 0.0f, 0.0f, 1.0f}
{
	width = 1.0f;
	height = 1.0f;


	std::vector<float> vbo = {
		// 3D position, 		Color coord					Texture coord
		 0.0f,   0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f,
		width, 	 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f,
		width, height, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f,
		 0.0f, height, 0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f
	};

	shader = new Shader("res/shaders/SimpleShader.glsl");
	//shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	GShape = new GenericShape(shader, MGL_QUADS, vbo, {3, 4, 2, 1});

	cam = new Camera({1.0, 1.0, 3.5}, {1.0, 1.0, 0.0});
	cam->SetFOV(90);

	texture = new Texture("res/textures/gunsalpha.png");
	//GShape->SetTexture(texture);

	scale = 1.5f;

	translation1 = glm::vec3(0, 0, 0);
	translation2 = glm::vec3(-0.5, -0.5, 0);
}
TestGAS::~TestGAS() {
	delete cam;
	delete texture;
	delete GShape;
	delete shader;
}

void TestGAS::onUpdate() {}

void TestGAS::onRender()
{	
	// Use our shader
	GShape->GetShader()->Bind();
	GShape->Scale(scale, scale, scale);

	GShape->SetPosition(translation1);
	mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * GShape->GetModelMat();
	GShape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	GShape->Draw();
	
	//GShape->SetColor({m_Color[0], 0.0f, 0.0f});
	GShape->SetPosition(translation2);

	mvp = cam->getProjectionMatrix() * cam->getViewMatrix() *  GShape->GetModelMat();
	GShape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	GShape->Draw();

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