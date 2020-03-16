#include "Core/Primitives/IndexBufferObject.hpp"

#include "Core/Debug/Debugger.hpp"

namespace MGLlib { 
namespace Primitives { 

IndexBufferObject::IndexBufferObject(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBufferObject::~IndexBufferObject(){
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBufferObject::Bind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBufferObject::UnBind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}}