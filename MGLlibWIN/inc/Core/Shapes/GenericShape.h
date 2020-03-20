#ifndef GENERICSHAPEHEADER
#define GENERICSHAPEHEADER

#include <vector>
#include <array>
#include <string>
#include "GenericAbstractShape.h"

namespace MGLlib{

	struct ColorNDC {
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;
		float A = 1.0f;
	};

	struct Color {
		int R = 255;
		int G = 255;
		int B = 255;
		int A = 255;
	};

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

typedef void (*SetColorFunc)(Shader* shader, ColorNDC RGBA);
void SetColorDefault(Shader* shader, ColorNDC RGBA);

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

	bool use_texture = false;
	bool CHcolor_on_render = true;
	bool CHtexture_on_render = true;

protected:
	ShapeType ST;

	GenericAbstractShape<float>* GAShape;
	SetColorFunc SCF = SetColorDefault;
	ColorNDC color = {1.0f, 1.0f, 1.0f, 1.0f};

	glm::vec3 Position = glm::vec3(0.0f);
	glm::mat4 ModelMat = glm::mat4(1.0f);
	glm::vec3 ScaleMat = glm::vec3(1.0f);
	glm::mat4 Translation = glm::mat4(1.0f);

	void GenerateGAS(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
	std::vector<unsigned int> GenerateIndexBuffer(ShapeType ST, unsigned int VertexCount);

public:
	//! @brief takes vector of points, layout should describe the layout of 1 vector
	//! @remark VertexLayout example: vector {X, Y, Z, R, G, B, A} layout : {3, 4}
	GenericShape(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
	//! @brief this constructor is used by the Shape class
	GenericShape(Shader* shader, ShapeType ST);
	~GenericShape();

	//! @brief returns the name of the shape
	std::string getName();
	
	//! @brief draws the shape
	//! @remark Before drawing bind the right shader
	void Draw(void);

	//! @brief sets scale of shape
	void Scale(float x, float y, float z = 1.0f);
	//! @brief Sets the color of the model
	//! @remark Color value between 0.0 and 1.0
	void SetColorNDC(ColorNDC RGBA);
	//! @brief Sets the color of the model
	//! @remark Color value between 0 and 255
	void SetColor(Color RGBA);
	//! @brief Sets the "SetColor" function, usefull when using custom shaders
	//! @remark this function should accept RGBA values between 0.0 and 1.0
	void SetSetColorFunc(SetColorFunc SCF);
	//! @brief When turned off the shape uses the color of last drawn shape
	void change_color_on_render(bool v){this->CHcolor_on_render = v;}
	//! @brief When turned off the shape uses the texture of the last drawn shape
	void change_texture_on_render(bool v){this->CHtexture_on_render = v;}
	//! @brief Sets the texture of the shape
	void SetTexture(Texture* texture, int slot = 0);
	//! @brief Disables the set texture
	void DisableTexture();
	//! @brief Enables the set texture
	void EnableTexture();
	
	//! @brief Sets the position of the object relative to the world
	void SetPosition(glm::vec3 position);
	//! @brief returns the objects position relative to the world
	glm::vec3 GetPosition(void);
	//! @brief returns the objects model matrix
	glm::mat4 GetModelMat(void);
	//! @brief Sets the objects model matrix;
	void SetModelMat(glm::mat4 modelmatrix);

	//! @brief Sets vertex data
	//! @remark This function is slow, if you only want to move the model use SetPosition();
	void SetVertexData(std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
	//! @brief Sets the Index Buffer of the shape
	//! @remark Call this function when using a [MGL_CUSTOM] with a special indexbufferobject;
	void SetIndexBuffer(std::vector<unsigned int> IndexBuffer);

	Shader* GetShader(void);
};

}	// namespace

#endif