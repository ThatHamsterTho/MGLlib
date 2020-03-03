#ifndef VBOHEADERGUARD
#define VBOHEADERGUARD

#define VertexBufferLength 8
union VertexBufferData { 
	struct{
		// position
		union { struct {
				float x, y;
			};
			float position[2];
		};
		// texture position
		union { struct {
				float Tx, Ty;
			};
			float TexturePosition[2];
		};
		// Color
        union { struct {
				float r, g, b, a;
			};
			float Color[4];
		};
    };
    float data[VertexBufferLength];
    VertexBufferData(){
		for(int i = 0; i < VertexBufferLength; i++){
			data[i] = 0.0f;
		}
	}
    unsigned int getSize(){
        return sizeof(data);
    }
	unsigned int ElementCount(){
		return VertexBufferLength;
	}
};

template<int N>
union VertexBufferDataArray {
	VertexBufferData VBD[N];
	float VBO[N * VertexBufferLength];
	VertexBufferDataArray(){}
	unsigned int getSize(){
        return sizeof(VBD);
    }
};

class VertexBuffer{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
 	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

#endif