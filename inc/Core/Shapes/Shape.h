#ifndef SHAPEHEADERGUARD
#define SHAPEHEADERGUARD

#include "GenericShape.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace MGLlib{

enum Usage_Type{
	Draw_Static,
	Draw_Dynamic
};

class Shape : public GenericShape
{
	protected:
	unsigned int VertexCount = 0;
	std::vector<std::array<float, 3>> Model;
	std::vector<std::array<float, 4>> ColorPerVector;
	std::vector<std::array<float, 2>> TextureCoords;
	std::vector<float>		  TextureSlots;
	
	Usage_Type uType = Draw_Dynamic;
	bool DataChanged = true;

	static GLFWwindow* window;
	Shader* shader;

	void UpdateVectorBuffers();
	void UpdateVertexCount(std::vector<float> vector, int VertexSize);
	void SetData(void);

	public:
		Shape(ShapeType ST, Shader* shader, Usage_Type uType = Draw_Dynamic);
		~Shape();

		void UpdateData(void);
		void Draw(void);

		void SetModelNDC(std::vector<std::array<float, 3>> Model){this->Model = Model;}
		void SetModel3D(std::vector<float> Model);
		void SetModel2D(std::vector<float> Model);
		void SetTextureCoord(std::vector<float> TextureCoords);
		void SetTextureSlots(std::vector<float> TextureSlots);
		void SetTextureSlot(unsigned int Vertex, float TextureSlot);
		void SetColorVec(unsigned int Vertex, std::array<float, 4> Color);
		void SetColorVecNDC(unsigned int Vertex, std::array<float, 4> Color);
		void SetVertex2D(unsigned int Vertex, std::array<float, 2> coor);
		void SetVertex3D(unsigned int Vertex, std::array<float, 3> coor);
		void SetVertex2D_NDC(unsigned int Vertex, std::array<float, 2> coor);
		void SetVertex3D_NDC(unsigned int Vertex, std::array<float, 3> coor);

		void SetWindowContext(GLFWwindow* window){this->window = window;}

		std::vector<std::array<float, 3>> GetModel(void){return this->Model;}
};

}
#endif