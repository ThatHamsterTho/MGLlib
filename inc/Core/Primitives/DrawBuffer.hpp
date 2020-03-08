#ifndef VERTEXBUFFERSTRUCTHEADERGUARD
#define VERTEXBUFFERSTRUCTHEADERGUARD

#include <vector>
#include "VertexBufferLayout.hpp"
#include "IndexBufferObject.hpp"

namespace MGLlib { 
namespace Primitives {

template<typename type>
class DrawBuffer{
	private:
		unsigned int* layout = nullptr;
		unsigned int  layoutSize;
		unsigned int* IndexBufferData = nullptr;
		unsigned int  IndexBufferSize;

		unsigned int VertexLength;
		unsigned int VertexCount;
		
		type* VertexDataArray = nullptr;

	// private methods
		void SetVertexLength(std::vector<unsigned int> layout){
			// get Vertex Length from layout
			VertexLength = 0;
			for(unsigned int i = 0; i < layout.size(); i++){
				VertexLength += layout[i];
			}
		}
		void DefaultConstructor(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout){	
			// create space for data
			VertexCount = array.size() / VertexLength;
			VertexDataArray = new float[VertexCount*VertexLength];
			// set vertex data
			SetData(array);
			// create IBO
			SetIBO(IndexBuffer);
			// create VBO layout
			SetLayout(layout);
		}

	public:
	// constructors
		DrawBuffer(unsigned int count, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			std::vector<type> data;
			for(int i = 0; i < count * VertexLength; i++){
				data.push_back(nullptr);
			}
			DefaultConstructor(data, IndexBuffer, layout);
		}
		DrawBuffer(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			DefaultConstructor(array, IndexBuffer, layout);
		}
		~DrawBuffer(void){
			delete [] VertexDataArray;
			delete [] IndexBufferData;
			delete [] layout;
		}

	// setters
		void SetData(const std::vector<type> array){
			if((array.size() % VertexLength)){
				#ifdef VBLAYOUTWARNING
					pWARNING("Vertex Data does not match given layout of %d values", VertexLength);
				#endif
			}
			for(unsigned int i = 0; i < array.size(); i++){
				this->VertexDataArray[i] = array[i];
			}
		}
		void SetIBO(std::vector<unsigned int> IndexBuffer){
			if(this->IndexBufferData){
				delete this->IndexBufferData;
			}
			IndexBufferData = new unsigned int[IndexBuffer.size()];
			for(unsigned int i = 0; i < IndexBuffer.size(); i++){
				IndexBufferData[i] = IndexBuffer[i];
			}
			IndexBufferSize = IndexBuffer.size();
		}
		void SetLayout(std::vector<unsigned int> layout){
			if(this->layout){
				delete this->layout;
				SetVertexLength(layout);
			}
			this->layout = new unsigned int[layout.size()];
			for(unsigned int i = 0; i < layout.size(); i++){
				this->layout[i] = layout[i];
			}
			layoutSize = layout.size();
		}

	// getters
		type* GetData(void){
			return VertexDataArray;
		}
		unsigned int* GetIBO(void){
			return IndexBufferData;
		}
		unsigned int GetIBOSize(void){
			return IndexBufferSize;
		}
		unsigned int* GetLayout(void){
			return layout;
		}
		unsigned int GetLayoutSize(void){
			return layoutSize;
		}

		unsigned int getSize(){
			return VertexCount * VertexLength * sizeof(type);
		}

		unsigned int getVertexCount(){
			return VertexCount;
		}
};

}} // namespace

#endif