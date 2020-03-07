#ifndef GASHEADERGUARD
#define GASHEADERGUARD

#include "DrawBuffer.hpp"

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "IndexBufferObject.hpp"

#include "Texture.hpp"
#include "Shader.hpp"

#include "Debugger.hpp"

namespace MGLlib { 

template<typename type>
class GenericAbstractShape{
	public:
		GenericAbstractShape(const GenericAbstractShape& GAS);
		GenericAbstractShape(Primitives::Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType = GL_TRIANGLES);
		~GenericAbstractShape();

		void Draw(void);
		void RenderTextures(void);
		void SetTexture(Primitives::Texture* texture, int slot = 0, bool UseDefaultShader = true);
		void DisableTexture(void);
		void SetIndexBuffer(unsigned int* IndexBuffer, unsigned int count);
		void SetVertexBuffer(const void* data, unsigned int size);
		void SetVertexLayout(unsigned int* layout, unsigned int count);
		void SetScale(type x, type y, type z);
		void SetShader(Primitives::Shader* shader);
		Primitives::Shader* GetShader(void);

	private:
		Primitives::VertexArrayObject* 	VA 		= nullptr;
		Primitives::VertexBufferObject* VB 		= nullptr;
		Primitives::VertexBufferLayout* Vlayout = nullptr;
		Primitives::IndexBufferObject* 	IB		= nullptr;
		Primitives::Texture* 			texture = nullptr;
		Primitives::Shader*				shader	= nullptr;
		// used for when no IBO is given
		unsigned int					Vertices;
		unsigned int					DrawType;
};


// template implementation

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(const GenericAbstractShape& GAS){
	this->VA = GAS.VA;
	this->Vlayout = GAS.Vlayout;
	this->IB = GAS.IB;
	this->texture = GAS.texture;
	this->shader = GAS.shader;
	this->use_texture = GAS.use_texture;
}

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Primitives::Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType)
	: VA(new Primitives::VertexArrayObject()), DrawType(DrawType)
{	
	this->shader = shader;

	VB = new Primitives::VertexBufferObject(data->GetData(), data->getSize());
	IB = new Primitives::IndexBufferObject(data->GetIBO(), data->GetIBOSize());
	Vlayout = new Primitives::VertexBufferLayout();
	unsigned int* layoutlocal = data->GetLayout();
	for(unsigned int i = 0; i < data->GetLayoutSize(); i++){
		Vlayout->Push<type>(layoutlocal[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
GenericAbstractShape<type>::~GenericAbstractShape(){
	delete VA;
	delete VB;
	delete IB;
	delete Vlayout;
}

template<typename type>
void GenericAbstractShape<type>::SetTexture(Primitives::Texture* texture, int slot, bool UseDefaultShader){
	this->texture = texture;
	shader->Bind();
	this->texture->Bind(slot);
	if(UseDefaultShader){
		shader->SetUniform1i("u_Texture", slot);
		shader->SetUniform1i("u_Use_Texture", true); // enable textures
	}
}

template<typename type>
void GenericAbstractShape<type>::DisableTexture(void){
	shader->Bind();
	//texture->Unbind();
	shader->SetUniform1i("u_Use_Texture", false); // disable textures
}

template<typename type>
void GenericAbstractShape<type>::SetIndexBuffer(unsigned int* IndexBuffer, unsigned int count){
	if(IB){
		delete IB;
	}
	this->IB = new Primitives::IndexBufferObject(IndexBuffer, count); 
}
template<typename type>
void GenericAbstractShape<type>::SetVertexBuffer(const void* data, unsigned int size){
	if(VB){
		delete VB;
	}
	VB = new Primitives::VertexBufferObject(data, size);
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetVertexLayout(unsigned int* layout, unsigned int count){
	if(Vlayout){
		delete Vlayout;
	}
	Vlayout = new Primitives::VertexBufferLayout();
	unsigned int* layoutlocal = layout;
	for(unsigned int i = 0; i < count; i++){
		Vlayout->Push<type>(layoutlocal[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetShader(Primitives::Shader* shader){
	this->shader = shader;
}

template<typename type>
void GenericAbstractShape<type>::SetScale(type x, type y, type z){
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

template<typename type>
Primitives::Shader* GenericAbstractShape<type>::GetShader(void){
	return this->shader;
}

template<typename type>
void GenericAbstractShape<type>::Draw(void){
	shader->Bind();
	VA->Bind();
	IB->Bind();
	GLCall(glDrawElements(DrawType, IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}

} // namespace

#endif
