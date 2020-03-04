#ifndef VBOHEADERGUARD
#define VBOHEADERGUARD

#define VertexBufferLength 10
struct VertexBufferData { 
	struct{
		// position
		union { struct {
				float x, y, z;
			};
			float position[3];
		};
		// texture position
		union { struct {
				float Tx, Ty, Tz;
			};
			float TexturePosition[3];
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