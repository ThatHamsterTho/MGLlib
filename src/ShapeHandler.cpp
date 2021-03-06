#include "ShapeHandler.h"

namespace MGLlib {
	Shape* ShapeHandler::baseShape = nullptr;

ShapeHandler::ShapeHandler(){}
ShapeHandler::~ShapeHandler(){
	if(this->shader){
		delete this->shader;
	}
	if(this->cam){
		delete this->cam;
	}
	if(this->baseShape){
		delete this->baseShape;
	}
}

void ShapeHandler::InitHandler(GLFWwindow* window){
	this->window = window;
	this->shader = new Shader("res/shaders/SimpleShader.glsl");
	this->cam = new Camera({0.0, 0.0, 1.0}, {0.0, 0.0, 0.0});
	// .618 is a magic number
	this->baseShape = new Shape(MGL_CUSTOM, shader, Draw_Static);
	baseShape->SetWindowContext(window);
}


void ShapeHandler::Enable3Drender(void){this->_3Drendering = true;}
void ShapeHandler::Disable3Drender(void){this->_3Drendering = false;}
void ShapeHandler::Enableortho(void){this->_OrthoRender = true;}
void ShapeHandler::Disableortho(void){this->_OrthoRender = false;}

void ShapeHandler::SetProjectionView(void){
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	if(_OrthoRender){
		vp = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 20.0f) * cam->getViewMatrix();
	}
	else{
		vp = cam->getProjectionMatrix() * cam->getViewMatrix();	
	}
}

void ShapeHandler::Draw(GenericShape* shape, void (*DrawFunc)(void)){
	if(_3Drendering){
		if(cam->CheckUpdated()){
			SetProjectionView();
		}
	}
	shader->Bind();
	if(DrawFunc){
		DrawFunc();
	}
	glm::mat4 mvp = shape->GetModelMat();
	if(_3Drendering){
		mvp = vp * shape->GetModelMat();
	}
	
	shape->GetShader()->SetuniformMat4f("u_MVP", mvp);
	shape->Draw();
}

void ShapeHandler::Draw(Shape* shape, void (*DrawFunc)(void)){
	shape->UpdateData();
	Draw(reinterpret_cast<GenericShape*>(shape), DrawFunc);
}

void ShapeHandler::SetDefaultShader(Shader* shader){
	this->shader = shader;
}

Camera* ShapeHandler::GetCamera(void){
	return this->cam;
}
Shader* ShapeHandler::GetGlobalShader(void){
	return this->shader;
}

// Generic Shapes
GenericShape* ShapeHandler::CreateGenShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout){
	return new GenericShape(this->shader, ST, VertexBuffer, VertexLayout);
}

GenericShape* ShapeHandler::CreateGenShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<int> VertexLayout){
	auto getPosLayoutVal = [](int VertexLayoutI)->unsigned int {
		if(VertexLayoutI < 0){
			switch (VertexLayoutI)
			{
				case NDC_XY:
					return 2;			
				case NDC_XYZ:
					return 3;
				case NDC_RGB:
					return 3;
				case NDC_RGBA:
					return 4;
				default:
					return VertexLayoutI * -1;
			}
		}
		else{
			return (unsigned int)(VertexLayoutI);
		}			
	};
	
	std::vector<float> NDCVertexBuffer;
	std::vector<unsigned int> NDCVertexLayout;
	int dim[3];
	glfwGetWindowSize(this->window, &dim[0], &dim[1]);
	dim[2] = dim[0];
	unsigned int VertexLength = 0;
	for(unsigned int i = 0; i < VertexLayout.size(); i++){
		VertexLength += getPosLayoutVal(VertexLayout[i]);
		NDCVertexLayout.push_back(getPosLayoutVal(VertexLayout[i]));
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
			if(VertexLayout[i] >= 0){
				for(int j = 0; j < VertexLayout[i]; j++){
					NDCVertexBuffer.push_back(VertexBuffer[v * VertexLength + VertexBase + j]);
				}
			}
			else{
				switch(VertexLayout[i]){
					case NDC_XY:
						for(unsigned int j = 0; j < 2; j++){
							if(VertexBuffer[v * VertexLength + VertexBase + j]){
								NDCVertexBuffer.push_back(2*(VertexBuffer[v * VertexLength + VertexBase + j])/((float)dim[j]));
							}
							else{
								NDCVertexBuffer.push_back(0.0f);
							}
						}
						break;
					case NDC_XYZ:
						for(unsigned int j = 0; j < 3; j++){
							if(VertexBuffer[v * VertexLength + VertexBase + j]){
								NDCVertexBuffer.push_back(2*(VertexBuffer[v * VertexLength + VertexBase + j])/((float)dim[j]));
							}
							else{
								NDCVertexBuffer.push_back(0.0f);
							}
						}
						break;
					case NDC_RGB:
						for(unsigned int j = 0; j < 3; j++){
							if(VertexBuffer[v * VertexLength + VertexBase + j]){
								NDCVertexBuffer.push_back((VertexBuffer[v * VertexLength + VertexBase + j])/(255));
							}
							else{
								NDCVertexBuffer.push_back(0.0f);
							}
						}						
						break;
					case NDC_RGBA:
						for(unsigned int j = 0; j < 4; j++){
							if(VertexBuffer[v * VertexLength + VertexBase + j]){
								NDCVertexBuffer.push_back((VertexBuffer[v * VertexLength + VertexBase + j])/(255));
							}
							else{
								NDCVertexBuffer.push_back(0.0f);
							}
						}						
						break;
					default:
						pWARNING("Unsupported C2NDC enum value");
						for(unsigned int j = 0; j < (unsigned int)(VertexLayout[i] * -1); i++){
							NDCVertexBuffer.push_back(VertexBuffer[v * VertexLength + VertexBase + j]);
						}
						break;
				}
			}
			VertexBase += NDCVertexLayout[i];
		}
	}
	return CreateGenShapeNDC(ST, NDCVertexBuffer, NDCVertexLayout);
}

// Shape
Shape* ShapeHandler::CreateShape(ShapeType ST, Usage_Type uType){
	Shape* SH = new Shape(ST, shader, uType);
	return SH;
}

// Rectangle
Rectangle* ShapeHandler::CreateRectangle(float x, float y, float z, float width, float height, float depth, bool tild, Usage_Type uType){
	Rectangle* R = new Rectangle(shader, uType);
	R->MakeShape(x, y, z, width, height, depth, tild);
	return R;
}
Rectangle* ShapeHandler::CreateRectangle(float x, float y, float width, float height, Usage_Type uType){
	return CreateRectangle(x, y, 0.0f, width, height, 0.0, uType);
}

// Circle
Circle* ShapeHandler::CreateCircle(float x, float y, float z, float Radius, int Sectors, Usage_Type uType){
	Circle* C = new Circle(shader, uType);
	C->MakeShape(x, y, z, Radius, Sectors);
	return C;
}
Circle* ShapeHandler::CreateCircle(float x, float y, float Radius, int Sectors, Usage_Type uType){
	return CreateCircle(x, y, 0.0f, Radius, Sectors, uType);
}

// Cube
Cube* ShapeHandler::CreateCube(float x, float y, float z, float width, float height, float depth, Usage_Type uType){
	Cube* C = new Cube(shader, uType);
	C->SetCube(x, y, z, width, height, depth);
	return C;
}

} // namespace