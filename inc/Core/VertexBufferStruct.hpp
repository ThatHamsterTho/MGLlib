#ifndef VERTEXBUFFERSTRUCTHEADERGUARD
#define VERTEXBUFFERSTRUCTHEADERGUARD

#include <iostream>
#include <vector>

#include "VertexBufferLayout.hpp"

#define VERTEXLENGTH 10

template<typename type>
union VertexData{
	// position
	union { struct {
			type x, y, z;
		};
		type position[3];
	};
	// texture position
	union { struct {
			type Tx, Ty, Tz;
		};
		type TexturePosition[3];
	};
	// Color
	union { struct {
			type r, g, b, a;
		};
		type Color[4];
	};
	type data[VERTEXLENGTH];
    
	VertexData(void){}
    unsigned int getSize(){
        return sizeof(data);
    }
	unsigned int ElementCount(){
		return VERTEXLENGTH;
	}
};

template<typename type>
class DrawBuffer{
	private:
		VertexBufferLayout* layout = nullptr;
		IndexBufferObject* ibo = nullptr;

		unsigned int VertexLength;
		unsigned int VertexCount;
		
		union{
			VertexData<type> *vertexdata = nullptr;
			type* VertexDataArray;
		};

		void SetVertexLength(std::vector<unsigned int> layout){
			// get Vertex Length from layout
			VertexLength = 0;
			for(unsigned int i = 0; i < layout.size(); i++){
				VertexLength += layout[i];
			}
		}
		void DefaultConstructor(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout){	
			// create space for data
			vertexdata = new VertexData<type>[VertexLength];
			VertexCount = array.size() / VertexLength;
			// set vertex data
			SetData(array);
			// create IBO
			SetIBO(IndexBuffer);
			// create VBO layout
			SetLayout(layout);
		}

	public:
	
		DrawBuffer(unsigned int count, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			std::vector<type> data;
			for(int i = 0; i < VertexLength; i++){
				data.push_back((type)0);
			}
			DefaultConstructor(data, IndexBuffer, layout);
		}
		DrawBuffer(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			DefaultConstructor(array, IndexBuffer, layout);
		}
		~DrawBuffer(void){
			delete vertexdata;
			delete ibo;
			delete layout;
		}

		void SetData(const std::vector<type> array){
			if((array.size() % VertexLength)){
				#ifdef VBLAYOUTWARNING
					printf("[WARNING]: Vertex Data does not match given layout of %d values", VertexLength);
				#endif
			}
			for(unsigned int i = 0; i < array.size(); i++){
				this->VertexDataArray[i] = array[i];
			}
		}
		void SetIBO(std::vector<unsigned int> IndexBuffer){
			if(this->ibo){
				delete this->ibo;
			}
			unsigned int *IndexBufferLocal = new unsigned int[IndexBuffer.size()];
			for(unsigned int i = 0; i < IndexBuffer.size(); i++){
				IndexBufferLocal[i] = IndexBuffer[i];
			}
			this->ibo = new IndexBufferObject(IndexBufferLocal, IndexBuffer.size());
		}
		void SetLayout(std::vector<unsigned int> layout){
			if(this->layout){
				delete this->layout;
				SetVertexLength(layout);
			}
			this->layout = new VertexBufferLayout();
			for(unsigned int i = 0; i < layout.size(); i++){
				this->layout->Push<type>(layout[i]);
			}
		}

		type* GetData(void){
			return VertexDataArray;
		}
		IndexBufferObject* GetIBO(void){
			return ibo;
		}
		VertexBufferLayout* GetLayout(void){
			return layout;
		}

		unsigned int getSize(){
			return VertexLength * vertexdata[0].ElementCount() * sizeof(type);
		}

		unsigned int getVertexCount(){
			return VertexCount;
		}
};

#endif