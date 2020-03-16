#ifndef VBOLAYOUTHEADERGUARD
#define VBOLAYOUTHEADERGUARD

#include "Core/Debug/Debugger.hpp"

#include <vector>

namespace MGLlib { 
namespace Primitives { 

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type){
		switch (type){
			case GL_BYTE:			return 1;
			case GL_SHORT:			return 2;
			case GL_INT:			return 4;
			case GL_FLOAT: 			return 4;
			case GL_DOUBLE:			return 8;
			case GL_UNSIGNED_BYTE: 	return 1;
			case GL_UNSIGNED_SHORT:	return 2;
			case GL_UNSIGNED_INT: 	return 4;
			
		}
		pERROR("unsupported buffer element type");
		return 0;
	}
};

class VertexBufferLayout{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
 	VertexBufferLayout();
 	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};


}} // namespace
#endif