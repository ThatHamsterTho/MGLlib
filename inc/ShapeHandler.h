#ifndef SHAPEHANDLER
#define SHAPEHANDLER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Shader.hpp"		// for default shader
#include "GenericShape.h"

namespace MGLlib {

typedef GenericShape Shape;

// Determines the conversion based on which length
enum C2NDC {
	NDC_NOT,
	NDC_WIDTH,
	NDC_HEIGHT
};

class ShapeHandler
{
	protected:
		Camera *cam;
		Primitives::Shader* shader;
		GLFWwindow* window;

	public:
		ShapeHandler();
		~ShapeHandler();
		
		void InitHandler(GLFWwindow* window);

		//! @brief creates a Shape based on coordinates between -1 and 1
		Shape* CreateShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout = {3});

		//! @brief creates a Shape based on coordinates between pixels, origin is at the bottom left
		//! @param VertexLayout boolean represents if that part of the layout should be normalized
		Shape* CreateShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<std::pair<unsigned int, C2NDC>> VertexLayout = {{3, NDC_WIDTH}});

		void SetDefaultShader(Primitives::Shader* shader);
		//! @brief draws the shape
		//! @param DrawFunc Performs this function between binding the shader and drawing the Shape
		void Draw(Shape* Shape, void (*DrawFunc)(void) = nullptr);

		Camera* GetCamera(void);
		Primitives::Shader* GetGlobalShader(void);
};

} // namespace
#endif