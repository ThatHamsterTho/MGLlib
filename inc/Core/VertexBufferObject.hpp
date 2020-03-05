#ifndef VBOHEADERGUARD
#define VBOHEADERGUARD

class VertexBufferObject{
private:
	unsigned int m_RendererID;

public:
	VertexBufferObject(const void* data, unsigned int size);
 	~VertexBufferObject();

	void Bind() const;
	void UnBind() const;

};

#endif