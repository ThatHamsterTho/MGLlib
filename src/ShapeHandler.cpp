#include "ShapeHandler.h"

namespace MGLlib {
ShapeHandler::ShapeHandler(){
	this->window = nullptr;
	this->shader = nullptr;
	this->cam 	 = nullptr;
}
ShapeHandler::~ShapeHandler(){
	delete this->shader;
	delete this->cam;
}

void ShapeHandler::InitHandler(GLFWwindow* window){
	this->window = window;
	this->shader = new Primitives::Shader("res/shaders/SimpleShader.glsl");
	this->cam = new Camera({0.0, 0.0, 0.619}, {0.0, 0.0, 0.0});
	// .619 is a magic number
}

Shape* ShapeHandler::CreateShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout){
	return new GenericShape(this->shader, ST, VertexBuffer, VertexLayout);
}

Shape* ShapeHandler::CreateShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<std::pair<unsigned int, C2NDC>> VertexLayout){
	std::vector<float> NDCVertexBuffer;
	std::vector<unsigned int> NDCVertexLayout;
	int dim[3];
	glfwGetWindowSize(this->window, &dim[1], &dim[2]);
	unsigned int VertexLength = 0;
	for(unsigned int i = 0; i < VertexLayout.size(); i++){
		VertexLength += VertexLayout[i].first;
		NDCVertexLayout.push_back(VertexLayout[i].first);
	}
	unsigned int VertexCount = 0; 
	if(VertexBuffer.size() % VertexLength){
		pWARNING("VertexBuffer does not match VertexLayout\n VertexBuffer Size: %lu : VertexLength: %u \n", VertexBuffer.size(), VertexLength);
	}
	else{
		VertexCount = VertexBuffer.size() / VertexLength;
	}
	unsigned int VertexBase;
	for(unsigned int v = 0; v < VertexCount; v++){
		VertexBase = 0;
		for(unsigned int i = 0; i < VertexLayout.size(); i++){
			if(VertexLayout[i].second){
				for(unsigned int j = 0; j < VertexLayout[i].first; j++){
					NDCVertexBuffer.push_back((2*VertexBuffer[VertexBase + j])/dim[VertexLayout[i].second] - 1);
				}
			}
			VertexBase += VertexLayout[i].first;
		}
	}
	return CreateShapeNDC(ST, NDCVertexBuffer, NDCVertexLayout);
}

void ShapeHandler::SetDefaultShader(Primitives::Shader* shader){
	this->shader = shader;
}

void ShapeHandler::Draw(Shape* Shape, void (*DrawFunc)(void)){
	shader->Bind();
	if(DrawFunc){
		DrawFunc();
	}
	glm::mat4 mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * Shape->GetModelMat();
	Shape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	Shape->Draw();
}

Camera* ShapeHandler::GetCamera(void){
	return this->cam;
}
Primitives::Shader* ShapeHandler::GetGlobalShader(void){
	return this->shader;
}

} // namespace