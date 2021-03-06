#include "Shape.h"

namespace MGLlib {

	GLFWwindow* Shape::window = nullptr;

	Shape::Shape(ShapeType ST, Shader* shader, Usage_Type uType) 
		: GenericShape(shader, ST){
		this->ST = ST;
		this->shader = shader;
		unsigned int layout[4] = {3, 4, 2, 1};
		this->GAShape->SetVertexLayout(layout, 4);
		this->uType = uType;
	}
	Shape::~Shape(){}

	void Shape::UpdateVectorBuffers(){
		for(unsigned int i = this->ColorPerVector.size(); i < VertexCount; i++){
			ColorPerVector.push_back({1.0f, 1.0f, 1.0f, 1.0f});
		}
		for(unsigned int i = this->TextureCoords.size(); i < VertexCount; i++){
			TextureCoords.push_back({1.0f, 1.0f});
		}
		for(unsigned int i = this->Model.size(); i < VertexCount; i++){
			Model.push_back({0.0f, 0.0f, 0.0f});
		}
		for(unsigned int i = this->TextureSlots.size(); i < VertexCount; i++){
			TextureSlots.push_back(0.0f);
		}
		DataChanged = true;
	}

	void Shape::UpdateVertexCount(std::vector<float> vector, int VertexSize){
		unsigned int Vcount = vector.size() / VertexSize;
		if(Vcount < this->VertexCount){
			if(this->VertexCount != 0){
				pWARNING("Less Vertices provided than present in buffer");
			}
		}
		if(Vcount > this->VertexCount){
			if(this->VertexCount != 0){
				pWARNING("Less Vertices provided than present in buffer");
			}
			VertexCount = Vcount;
		}
	}

	void Shape::SetModel2D(std::vector<float> Model){
		std::vector<float> VertexBuffer;
		for(unsigned int i = 0; i < Model.size() / 2; i++){
			VertexBuffer.push_back(Model[0]);
			VertexBuffer.push_back(Model[1]);
			VertexBuffer.push_back(0.0f);
		}
		SetModel3D(VertexBuffer);
	}

	void Shape::SetModel3D(std::vector<float> Model){
		//std::vector<std::array<float, 3>> NDCVertexBuffer;
		int dim[3];
		if(window){
			glfwGetWindowSize(this->window, &dim[0], &dim[1]);
		}
		else{
			dim[0] = 1.0f; dim[1] = 1.0f;
			pERROR("No window context set for Shape class");
		}
		dim[2] = dim[0];
		UpdateVertexCount(Model, 3);
		UpdateVectorBuffers();
		for(unsigned int i = 0; i < Model.size() / 3; i++){
			//NDCVertexBuffer.push_back({0.0f, 0.0f, 0.0f});
			for(unsigned int j = 0; j < 3; j++){
				if(Model[i * 3 + j]){
					this->Model[i][j] = (2*(Model[i * 3 + j])/((float)dim[j]));
				}
				else{
					this->Model[i][j] = 0.0f;
				}
			}
		}
		//this->Model = NDCVertexBuffer;
	}

	void Shape::SetTextureCoord(std::vector<float> TextureCoords){
		UpdateVertexCount(TextureCoords, 2);
		UpdateVectorBuffers();
		for(unsigned int i = 0; i < TextureCoords.size() / 2; i++){
			for(unsigned int j = 0; j < 2; j++){
				this->TextureCoords[i][j] = TextureCoords[i * 2 + j];
			}
		}
		DataChanged = true;
	}
	void Shape::SetTextureSlots(std::vector<float> TextureSlots){
		UpdateVertexCount(TextureSlots, 1);
		UpdateVectorBuffers();
		for(unsigned int i = 0; i < TextureSlots.size(); i++){
			this->TextureSlots[i] = TextureSlots[i];
		}
		DataChanged = true;
	}

	void Shape::SetTextureSlot(unsigned int Vertex, float TextureSlot){
		if(Vertex >= VertexCount){
			this->TextureSlots.push_back(TextureSlot);
			VertexCount++;
		}
		else{
			this->TextureSlots[Vertex] = TextureSlot;
		}
		UpdateVectorBuffers();
		DataChanged = true;
	}

	void Shape::SetColorVec(unsigned int Vertex, std::array<float, 4> Color){
		std::array<float, 4> ColorNDC;
		for(int i = 0; i < 4; i++){
			if(Color[i]){
				ColorNDC[i] = Color[i]/255;
			}else{
				ColorNDC[i] = 0.0f;
			}
		}
		SetColorVecNDC(Vertex, ColorNDC);
	}
	void Shape::SetColorVecNDC(unsigned int Vertex, std::array<float, 4> Color){
		for(int i = 0; i < 4; i++){
			this->ColorPerVector[Vertex][i] = Color[i];
		}
		DataChanged = true;
	};

	void Shape::SetVertex2D(unsigned int Vertex, std::array<float, 2> coor){
		SetVertex3D(Vertex, {coor[0], coor[1], 0.0f});
	}
	void Shape::SetVertex3D(unsigned int Vertex, std::array<float, 3> coor){
		std::array<float, 3> NDC_coor;
		int dim[3];
		if(window){
			glfwGetWindowSize(this->window, &dim[0], &dim[1]);
		}
		else{
			dim[0] = 1.0f; dim[1] = 1.0f;
			pERROR("No window context set for Shape class");
		}
		dim[2] = dim[0];
		for(int i = 0; i < 3; i++){
			NDC_coor[i] = (2*coor[i])/(float)dim[i] - 1.0f;
		}
		SetVertex3D_NDC(Vertex, NDC_coor);
	}
	void Shape::SetVertex2D_NDC(unsigned int Vertex, std::array<float, 2> coor){
		SetVertex3D_NDC(Vertex, {coor[0], coor[1], 0.0f});
	}
	void Shape::SetVertex3D_NDC(unsigned int Vertex, std::array<float, 3> coor){
		if(Vertex >= VertexCount){
			this->Model.push_back(coor);
			VertexCount++;
		}
		else{
			for(unsigned int i = 0; i < 3; i++){
				this->Model[Vertex][i] = coor[i];
			}
		}
		DataChanged = true;
		UpdateVectorBuffers();
	}

	void Shape::SetData(void){
		float* VertexDataArray = new float[VertexCount*10];
		for(unsigned int i = 0; i < VertexCount; i++){
			// push model
			for(int M = 0; M < 3; M++){
				VertexDataArray[i*10+M] = (Model[i][M]);
			}
			// push color
			for(int C = 0; C < 4; C++){
				VertexDataArray[i*10+C+3] = (ColorPerVector[i][C]);
			}
			// push texture
			for(int T = 0; T < 2; T++){
				VertexDataArray[i*10+T+7] = (TextureCoords[i][T]);
			}
			// push texture slots
			VertexDataArray[i*10+9] = TextureSlots[i];
		}
		
		if(uType == Draw_Static){
			Model.clear();
			TextureCoords.clear();
			ColorPerVector.clear();
		}

		// settings GenericAbstractShape VBO;
		this->GAShape->SetVertexBuffer(VertexDataArray, sizeof(float) * VertexCount * 10);
		delete [] VertexDataArray;

		// settings GenericAbstractShape IBO;
		std::vector<unsigned int> IndexBuffer = GenerateIndexBuffer(ST, VertexCount);
		unsigned int *IndexBufferData = new unsigned int[IndexBuffer.size()];
		for(unsigned int i = 0; i < IndexBuffer.size(); i++){
			IndexBufferData[i] = IndexBuffer[i];
		}
		this->GAShape->SetIndexBuffer(IndexBufferData, IndexBuffer.size());
		delete [] IndexBufferData;
		//GenerateGAS(shader, ST, VBO, {3, 2, 4});
	}

	void Shape::UpdateData(void){
		if(DataChanged){
			SetData();
			DataChanged = false;
		}
	}

	void Shape::Draw(void){
		GenericShape::Draw();
	}
}
