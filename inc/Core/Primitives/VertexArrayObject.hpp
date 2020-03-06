#ifndef VAOHEADERGUARD
#define VAOHEADERGUARD

#include "VertexBufferObject.hpp"
#include "VertexBufferLayout.hpp"

namespace MGLlib { 
namespace Primitives { 

class VertexArrayObject{
private:
	unsigned int m_RendererID;
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AddBuffer(const VertexBufferObject* vb, const VertexBufferLayout* layout);

	void Bind() const;
	void UnBind() const;
};

}} // namespace

#endif