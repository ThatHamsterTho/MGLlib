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
		GenericAbstractShape(Primitives::Shader* shader, Primitives::DrawBuffer<type>* data);
		~GenericAbstractShape();

		void Draw(void);
		void SetTexture(Primitives::Texture* texture, int slot = 0);
		void RemoveTexture(void);
		void SetScale(type x, type y, type z);
		void SetShader(Primitives::Shader* shader);
		Primitives::Shader* GetShader(void);

	private:
		Primitives::VertexArrayObject* 	VA;
		Primitives::VertexBufferObject* VB;
		Primitives::VertexBufferLayout* Vlayout;
		Primitives::IndexBufferObject* 	IB;
		Primitives::Texture* 			texture;
		Primitives::Shader*				shader;
		// used for when no IBO is given
		unsigned int					Vertices;
		bool 							use_texture;
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
GenericAbstractShape<type>::GenericAbstractShape(Primitives::Shader* shader, Primitives::DrawBuffer<type>* data)
	: VA(new Primitives::VertexArrayObject()), use_texture(false)
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
void GenericAbstractShape<type>::SetTexture(Primitives::Texture* texture, int slot){
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
void GenericAbstractShape<type>::SetShader(Primitives::Shader* shader){
	delete this->shader;
	this->shader = shader;
}

template<typename type>
void GenericAbstractShape<type>::SetScale(type x, type y, type z){
	shader->Bind();
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

template<typename type>
Primitives::Shader* GenericAbstractShape<type>::GetShader(void){
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

} // namespace

#endif
