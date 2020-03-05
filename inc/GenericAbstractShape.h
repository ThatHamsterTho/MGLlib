#ifndef GASHEADERGUARD
#define GASHEADERGUARD

#include "AbstGenericAbstractShape.h"

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(const GenericAbstractShape& GAS){
	this->VA = GAS.VA;
	this->IB = GAS.IB;
	this->texture = GAS.texture;
	this->shader = GAS.shader;
	this->use_texture = GAS.use_texture;
}

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, DrawBuffer<type>* data)
	: VA(new VertexArrayObject()), IB(data->GetIBO()), use_texture(false)
{	
	this->shader = shader;

	VertexBufferObject *VB = new VertexBufferObject(data->GetData(), data->getSize());

	VA->AddBuffer(VB, data->GetLayout());
}

template<typename type>
GenericAbstractShape<type>::~GenericAbstractShape(){
	delete shader;
	delete VA;
}

template<typename type>
void GenericAbstractShape<type>::SetTexture(Texture* texture, int slot){
	this->texture = texture;
	this->shader->Bind();
	this->texture->Bind(slot);
	shader->Bind();
	shader->SetUniform1i("u_Texture", 0);
	shader->SetUniform1i("u_Use_Texture", true); // enable textures
	use_texture = true;
}

template<typename type>
void GenericAbstractShape<type>::RemoveTexture(void){
	shader->Bind();
	shader->SetUniform1i("u_Use_Texture", false); // enable textures
	use_texture = false;
}

template<typename type>
void GenericAbstractShape<type>::SetShader(Shader* shader){
	delete this->shader;
	this->shader = shader;
}

template<typename type>
void GenericAbstractShape<type>::SetScale(type x, type y, type z){
	shader->Bind();
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

template<typename type>
Shader* GenericAbstractShape<type>::GetShader(void){
	shader->Bind();
	return this->shader;
}

template<typename type>
void GenericAbstractShape<type>::Draw(void){
	shader->Bind();
	VA->Bind();
	IB->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}


#endif
