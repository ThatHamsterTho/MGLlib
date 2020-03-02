#include "Renderer.hpp"


Renderer::Renderer() {}
Renderer::~Renderer(){}


void Renderer::Draw(const VertexArray *va, const IndexBuffer *ib, const Shader *shader) const{
	// Use our shader
	shader->Bind();

	// bind the vao and ibo
	va->Bind();
	ib->Bind();

	// Draw the triangle !
	GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr)); // 3 indices starting at 0 -> 1 triangle
}

void Renderer::Clear(unsigned int ClearBufferBit){
	GLCall(	glClear( ClearBufferBit ););
}