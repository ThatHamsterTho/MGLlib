#include "VertexArray.hpp"

#include "Debugger.hpp"

VertexArray::VertexArray(){
    GLCall(glGenVertexArrays(1, &m_RendererID));
}
VertexArray::~VertexArray(){
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer* vb, const VertexBufferLayout* layout){
    Bind();
	vb->Bind();
    const auto& elements = layout->GetElements();
    unsigned int offset = 0;
    for(unsigned int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(
            i,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            element.count,      // size
            element.type,       // type
            element.normalized, // normalized?
            layout->GetStride(),// stride
            reinterpret_cast<const void *>(offset) // array buffer offset
        ));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const{
    GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::UnBind() const{
    GLCall(glBindVertexArray(0));
}