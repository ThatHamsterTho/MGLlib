#include "GenericAbstractShape.h"
#include "Debugger.hpp"

GenericAbstractShape::GenericAbstractShape(VertexBuffer* VB, VertexBufferLayout* VBL, IndexBuffer* IB, usage type)
	: VA(new VertexArray()), IB(IB), use_texture(false), type(type)
{
	if(type == DYNAMIC_DRAW){
		this->VB = VB;
	}
	VA->AddBuffer(VB, VBL);
	// TODO: config this as default shader, (change file path?)
	shader = new Shader("res/shaders/SimpleVert.vert", "res/shaders/SimpleFrag.frag");
}
GenericAbstractShape::~GenericAbstractShape(){
	delete shader;
	delete VA;
}

void GenericAbstractShape::SetTexture(Texture* texture, int slot){
	this->texture = texture;
	this->shader->Bind();
	this->texture->Bind(slot);
	shader->Bind();
	shader->SetUniform1i("u_Texture", 0);
	shader->SetUniform1i("u_Use_Texture", true); // enable textures
	use_texture = true;
}

void GenericAbstractShape::RemoveTexture(void){
	shader->Bind();
	shader->SetUniform1i("u_Use_Texture", false); // enable textures
	use_texture = false;
}

void GenericAbstractShape::SetShader(Shader* shader){
	delete this->shader;
	this->shader = shader;
}

void GenericAbstractShape::SetScale(float x, float y, float z){
	shader->Bind();
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

Shader* GenericAbstractShape::GetShader(void){
	shader->Bind();
	return this->shader;
}

void GenericAbstractShape::Draw(void){
	shader->Bind();
	VA->Bind();
	IB->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}