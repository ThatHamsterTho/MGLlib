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
		GenericAbstractShape(Shader* shader, unsigned int DrawType = GL_TRIANGLES);
		GenericAbstractShape(const GenericAbstractShape& GAS);
		GenericAbstractShape(Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType = GL_TRIANGLES);
		~GenericAbstractShape();

		void Draw(void);
		void RenderTextures(void);
		void SetTexture(Texture* texture, int slot = 0, bool UseDefaultShader = true);
		void DisableTexture(void);
		void EnableTexture(void);
		void BindTexture(void);
		void SetIndexBuffer(unsigned int* IndexBuffer, unsigned int count);
		void SetVertexBuffer(const void* data, unsigned int size);
		void SetVertexLayout(unsigned int* layout, unsigned int count);
		void SetScale(type x, type y, type z);
		void SetShader(Shader* shader);
		Shader* GetShader(void);

	private:
		Primitives::VertexArrayObject* 	VA 		= nullptr;
		Primitives::VertexBufferObject* VB 		= nullptr;
		Primitives::VertexBufferLayout* Vlayout = nullptr;
		Primitives::IndexBufferObject* 	IB		= nullptr;
		Texture* 						texture = nullptr;
		unsigned int 					TextureSlot = 0;
		bool 							UseDefaultShader = false;
		Shader*							shader	= nullptr;
		// used for when no IBO is given
		unsigned int					Vertices;
		unsigned int					DrawType;
};


// template implementation

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, unsigned int DrawType)
	: VA(new Primitives::VertexArrayObject()), DrawType(DrawType)
{
	float data[] = {0.0f};
	VB = new Primitives::VertexBufferObject(data, sizeof(float));
	Vlayout = new Primitives::VertexBufferLayout();
	this->shader = shader;
}

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
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType)
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
void GenericAbstractShape<type>::SetTexture(Texture* texture, int slot, bool UseDefaultShader){
	this->texture = texture;
	this->TextureSlot = slot;
	this->UseDefaultShader = UseDefaultShader;
}

template<typename type>
void GenericAbstractShape<type>::DisableTexture(void){
	shader->Bind();
	//texture->Unbind();
	shader->SetUniform1i("u_Use_Texture", false); // disable textures
}

template<typename type>
void GenericAbstractShape<type>::EnableTexture(void){
	shader->Bind();
	shader->SetUniform1i("u_Use_Texture", true); // disable textures
}

template<typename type>
void GenericAbstractShape<type>::BindTexture(void){
	shader->Bind();
	this->texture->Bind(TextureSlot);
	if(UseDefaultShader){
		shader->SetUniform1i("u_Texture", TextureSlot);
		shader->SetUniform1i("u_Use_Texture", true); // enable textures
	}
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
	if(VA){
		delete VA;
	}
	VA = new Primitives::VertexArrayObject();
	VB = new Primitives::VertexBufferObject(data, size);
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetVertexLayout(unsigned int* layout, unsigned int count){
	if(Vlayout){
		delete Vlayout;
	}
	if(VA){
		delete VA;
	}
	VA = new Primitives::VertexArrayObject();
	Vlayout = new Primitives::VertexBufferLayout();
	unsigned int* layoutlocal = layout;
	for(unsigned int i = 0; i < count; i++){
		Vlayout->Push<type>(layoutlocal[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetShader(Shader* shader){
	this->shader = shader;
}

template<typename type>
void GenericAbstractShape<type>::SetScale(type x, type y, type z){
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

template<typename type>
Shader* GenericAbstractShape<type>::GetShader(void){
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
