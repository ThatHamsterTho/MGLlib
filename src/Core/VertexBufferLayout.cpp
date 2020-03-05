#include "VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout() : m_Stride(0){}
VertexBufferLayout::~VertexBufferLayout(){}

template <> void VertexBufferLayout::Push<char>(unsigned int count){
    m_Elements.push_back({GL_BYTE, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_BYTE);
}

template <> void VertexBufferLayout::Push<short>(unsigned int count){
    m_Elements.push_back({GL_SHORT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_SHORT);
}

template <> void VertexBufferLayout::Push<int>(unsigned int count){
    m_Elements.push_back({GL_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
}

template <> void VertexBufferLayout::Push<long>(unsigned int count){
    m_Elements.push_back({GL_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
}

template <> void VertexBufferLayout::Push<float>(unsigned int count){
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template <> void VertexBufferLayout::Push<double>(unsigned int count){
    m_Elements.push_back({GL_DOUBLE, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_DOUBLE);
}



template <> void VertexBufferLayout::Push<unsigned char>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

template <> void VertexBufferLayout::Push<unsigned short>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_SHORT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_SHORT);
}

template <> void VertexBufferLayout::Push<unsigned int>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template <> void VertexBufferLayout::Push<unsigned long>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}