#ifndef GENERICSHAPEHEADER
#define GENERICSHAPEHEADER

#include <vector>
#include <array>
#include <string>
#include "GenericAbstractShape.h"
#include "DrawBuffer.hpp"

namespace MGLlib{

	enum ShapeType {
		MGL_CUSTOM,
		MGL_POINTS,
		MGL_LINES,
		MGL_LINE_STRIP,
		MGL_LINE_LOOP,
		MGL_TRIANGLES,
		MGL_TRIANGLE_STRIP,
		MGL_TRIANGLE_FAN,
		MGL_POLYGON = MGL_TRIANGLE_FAN,
		MGL_QUADS,
		MGL_QUAD_STRIP,
		// keep this as last type
		__SHAPETYPECOUNT
	};

typedef void (*SetColorFunc)(Primitives::Shader* shader, std::array<float, 4> RGBA);
void SetColorDefault(Primitives::Shader* shader, std::array<float, 4> RGBA);

class GenericShape
{
	private:
	struct ShapeMapItem{
		ShapeType ST;
		std::string ShapeName;
		unsigned int GLtype;
		std::vector<unsigned int> IndexBufferBase;
		unsigned int baseCount;
		unsigned int extendCount;
	};
	static const ShapeMapItem ShapeMap[__SHAPETYPECOUNT];

	unsigned int VertexLength;	// how many floats in 1 vertex
	unsigned int VertexCount;	// how many vertexes in the buffer

	ShapeType ST;

	GenericAbstractShape<float>* GAShape;
	SetColorFunc SCF = SetColorDefault;

	glm::mat4 Model;

	public:
		//! @brief takes vector of points, layout should describe the layout of 1 vector
		//! @remark VertexLayout example: vector {X, Y, Z, R, G, B, A} layout : {3, 4}
		GenericShape(Primitives::Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
		~GenericShape();

		//! @brief returns the name of the shape
		std::string getName();
		

		//! @brief sets scale of shape
		void Scale(float x, float y, float z = 1.0f);
		//! @brief Sets the texture of the shape
		void SetTexture(Primitives::Texture* texture, int slot = 0);
		//! @brief Disables the texture
		void DisableTexture();
		//! @brief Sets the color of the model
		void SetColor(std::array<float, 4> RGBA);

		//! @brief Sets the "SetColor" function, usefull when using custom shaders
		void SetSetColorFunc(SetColorFunc SCF);
		
		//! @brief Sets the position of the object relative to the world
		void SetPosition(std::vector<float> model);
		//! @brief Sets vertex data
		//! @remark This function is slow, if you only want to move the model use SetPosition();
		void SetVertexData(std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
		//! @brief Sets the Index Buffer of the shape
		//! @remark Call this function when using a [MGL_CUSTOM] with a special indexbufferobject;
		void SetIndexBuffer(std::vector<unsigned int> IndexBuffer);

		//! @brief draws the shape
		void Draw(void);

		// TODO: remove?
		Primitives::Shader* GetShader(void);
};

}	// namespace

#endif