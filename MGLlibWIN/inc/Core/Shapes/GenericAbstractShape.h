#ifndef GASHEADERGUARD
#define GASHEADERGUARD

#include "Core/Primitives/VertexArrayObject.hpp"
#include "Core/Primitives/VertexBufferObject.hpp"
#include "Core/Primitives/IndexBufferObject.hpp"

#include "Core/Shapes/Texture.hpp"
#include "Core/Shapes/Shader.hpp"

#include "Core/Debug/Debugger.hpp"

namespace MGLlib { 

template<typename type>
class GenericAbstractShape{
	public:
		GenericAbstractShape(Shader* shader, unsigned int DrawType = GL_TRIANGLES);
		//! @brief Creates a GenericAbstractShape based on the provided VertexBuffer, VertexLayout and IndexBuffer
		//! @param VertexBuffer array of data points
		//! @param VertexBufferSize size of VertexBuffer in BYTES
		//! @param VertexLayout array that describes the VertexBuffer layout
		//! @param VertexLayoutCount VertexLayout element count
		//! @param IndexBuffer array that describes the order to use the given Vertices in
		//! @param IndexBufferCount IndexBuffer element count
		GenericAbstractShape(Shader* shader, type* VertexBuffer, unsigned int VertexBufferSize, unsigned int* VertexLayout, unsigned int VertexlayoutCount, unsigned int* IndexBuffer, unsigned int IndexBufferCount, unsigned int DrawType = GL_TRIANGLES);
		
		~GenericAbstractShape();

		void Draw(void);
		void RenderTextures(void);
		void SetTexture(Texture* texture, unsigned int slot = 0, bool UseDefaultShader = true);
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
		void GenerateGASObjects(void* VBO, unsigned int VBOsize, unsigned int* VBOlayout, unsigned int VBOlayoutCount, unsigned int* IBO, unsigned int IBOCount);
		Primitives::VertexArrayObject* 	VA 		= nullptr;
		Primitives::VertexBufferObject* VB 		= nullptr;
		Primitives::VertexBufferLayout* Vlayout = nullptr;
		Primitives::IndexBufferObject* 	IB		= nullptr;
		std::vector<Texture*>			texture = {nullptr};
		bool 							UseDefaultShader = false;
		Shader*							shader	= nullptr;
		// used for when no IBO is given
		unsigned int					Vertices;
		unsigned int					DrawType;
};

template<typename type>
void GenericAbstractShape<type>::GenerateGASObjects(void* VBO, unsigned int VBOsize, unsigned int* VBOlayout, unsigned int VBOlayoutCount, unsigned int* IBO, unsigned int IBOCount){
	VA = new Primitives::VertexArrayObject();
	if(VBO){
		VB = new Primitives::VertexBufferObject(VBO, VBOsize);
	}
	else{
		type data[] = {0.0f};
		VB = new Primitives::VertexBufferObject(data, VBOsize);
	}
	if(IBO){
		IB = new Primitives::IndexBufferObject(IBO, IBOCount);
	}
	else{
		unsigned int IBempty[] = {0};
		IB = new Primitives::IndexBufferObject(IBempty, 0);
	}
	Vlayout = new Primitives::VertexBufferLayout();
	for(unsigned int i = 0; i < VBOlayoutCount; i++){
		Vlayout->Push<type>(VBOlayout[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}


// template implementation
template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, unsigned int DrawType)
	: VA(new Primitives::VertexArrayObject()), DrawType(DrawType)
{
	this->shader = shader;
	// generate empty GAS
	GenerateGASObjects(nullptr, 0, nullptr, 0, nullptr, 0);
}

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, type* VertexBuffer, unsigned int VertexBufferSize, unsigned int* VertexLayout, unsigned int VertexlayoutCount, unsigned int* IndexBuffer, unsigned int IndexBufferCount, unsigned int DrawType){
	this->shader = shader;
	this->DrawType = DrawType;
	GenerateGASObjects(VertexBuffer, VertexBufferSize, VertexLayout, VertexlayoutCount, IndexBuffer, IndexBufferCount);
}


template<typename type>
GenericAbstractShape<type>::~GenericAbstractShape(){
	delete VA;
	delete VB;
	delete IB;
	delete Vlayout;
}

template<typename type>
void GenericAbstractShape<type>::SetTexture(Texture* texture, unsigned int slot, bool UseDefaultShader){
	if(slot >= this->texture.size()){
		this->texture.push_back(texture);
	}
	else{
		this->texture[slot] = texture;
	}
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
	for(unsigned int i = 0; i < texture.size(); i++){
		this->texture[i]->Bind(i);
		if(UseDefaultShader){
			shader->SetUniform1i("u_Texture", i);
			shader->SetUniform1i("u_Use_Texture", true); // enable textures
		}
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
	if(UseDefaultShader){
		shader->SetUniform4f("u_Scale", x, y, z, 1.0);
	}
	else{
		pWARNING("Scale function not available when using custom shader.");
	}
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
