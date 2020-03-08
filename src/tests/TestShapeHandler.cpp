#include "TestShapeHandler.h"

#include "ImGui/imgui.h"

namespace MGLlib {
using namespace Primitives;

namespace test {
TestShapeHandler::TestShapeHandler(Window* window) : Test(window), m_Color{0.0f, 0.0f, 0.0f, 1.0f}
{
	width = 400.0f;
	height = 400.0f;

/*
	std::vector<float> vbo = {
		// 3D position, 			Texture coord	Color coord
		 0.0f,   0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		width, 	 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		width, height, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		 0.0f, height, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f
	};
*/

	std::vector<float> vbo = {
		// 3D position, 			Texture coord	Color coord
		 0.0f,   0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f, 0.2f,
		width, 	 0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f, 0.2f,	
		width, height, 0.0f,		1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.2f,
		 0.0f, height, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.2f
	};

	//shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	texture = new Texture("res/textures/gunsalpha.png");
	//GShape->SetTexture(texture);

	scale = 1.0f;

	translation1 = glm::vec3(-1.0, -1.0,  0.0);
	translation2 = glm::vec3( 0.0,  0.0,  0.0);
	translation3 = glm::vec3(-0.5, -0.5,  0.0);

	GSH = window->CreateGenShape(MGL_QUADS, vbo, {NDC_XYZ, 2, 4});
	GSH->SetTexture(texture);

	SH = window->CreateShape(MGL_QUADS);
	SH->SetModel3D({
		  0.0f,    0.0f, 0.0f,
		 width,    0.0f, 0.0f,
		 width,  height, 0.0f,
		  0.0f,  height, 0.0f
	});

	rec = window->CreateRectangle(0.0f, 0.0f, width, height);
	rec->SetColor({255, 0, 0, 10});

	circ = window->CreateCircle(0.0f, 0.0f, 200.0f, 360);
	circ->SetColor({0, 255, 0, 128});

	cube = window->CreateCube(-200.0f, -200.0f, -200.0f, 400.0f, 400.0f, 400.0f);
	cube->SetColor({0, 0, 255, 64});
}
TestShapeHandler::~TestShapeHandler() {
	delete texture;
	delete GSH;
	delete SH;
	delete rec;
	delete circ;
	delete cube;
}

void TestShapeHandler::onUpdate() {}

void TestShapeHandler::onRender()
{	
	GSH->SetPosition(translation3);
	window->Draw(GSH);
	
	circ->SetPosition(translation2);
	window->Draw(circ);

	SH->SetPosition(translation1);
	window->Draw(SH);

	//cube->SetPosition(translation1);
	window->Draw(cube);
	
	// change color
	if (m_Color[0] > 1.0)
	{
		increment *= -1;
	}
	else if (m_Color[0] < 0.0)
	{
		increment *= -1;
	}

	//m_Color[0] += increment;
}

void TestShapeHandler::onImGuiRender()
{
	// draw ImGui window
	ImGui::SliderFloat3("Translation1", &translation1.x, -1.0f, 1.0f);
	ImGui::SliderFloat3("Translation2", &translation2.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Scale", &scale, 0.0f, 10.0f);
	ImGui::SliderFloat3("Camera", &window->GetCamera()->getPosition()[0], -5.0f, 5.0f);
	ImGui::SliderFloat3("Lookat", &window->GetCamera()->getLookat()[0], -5.0f, 5.0f);
	ImGui::SliderFloat("Perspective FOV", &FOV, 0.0f, 180.0f);
	ImGui::SliderFloat2("Perspective Z", &Z[0], 0.0f, 10.0f);
	ImGui::SliderFloat3("UP", &up[0], -1.0f, 1.0f);
	ImGui::SliderFloat2("Window dim", &dim[0], 0.0f, 800.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	window->GetCamera()->SetFOV(FOV);
	window->GetCamera()->SetPlane(Z[0], Z[1]);
	window->GetCamera()->SetUp({up[0], up[1], up[2]});
	window->GetCamera()->UpdateAspectRatio(dim[0], dim[1]);

	window->GetCamera()->UpdateCamera();
	window->GetCamera()->UpdatePerspective();
}

} // namespace test
} // namespace MGLlib