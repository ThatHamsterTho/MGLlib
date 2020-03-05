#ifndef IBOHEADERGUARD
#define IBOHEADERGUARD

class IndexBufferObject{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
 	IndexBufferObject(const unsigned int* data, unsigned int count);
 	~IndexBufferObject();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

#endif