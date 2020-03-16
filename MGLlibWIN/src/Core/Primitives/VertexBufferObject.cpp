#include "Core/Primitives/VertexBufferObject.hpp"

#include "Core/Debug/Debugger.hpp"

namespace MGLlib { 
namespace Primitives { 

VertexBufferObject::VertexBufferObject(const void* data, unsigned int size){
    GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBufferObject::~VertexBufferObject(){
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBufferObject::Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBufferObject::UnBind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

}} // namespace
