#ifndef SHAPEHANDLER
#define SHAPEHANDLER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Shader.hpp"		// for default shader
#include "GenericShape.h"

namespace MGLlib {

// Determines the conversion based on which length
enum C2NDC : int {
	NDC_XY		= -1,
	NDC_XYZ		= -2,
	NDC_RGB 	= -3,
	NDC_RGBA 	= -4,
};

class ShapeHandler
{
	private:
		glm::mat4 vp;

		void SetProjectionView(void);

	protected:
		Camera*		cam		= nullptr;
		Shader*		shader	= nullptr;
		GLFWwindow* window	= nullptr;
		bool		_3Drendering = false;
		bool		_OrthoRender = false;

	public:
		ShapeHandler();
		~ShapeHandler();

		void Enable3Drender(void);
		void Disable3Drender(void);
		void Enableortho(void);
		void Disableortho(void);
		
		void InitHandler(GLFWwindow* window);

		//! @brief creates a Shape based on coordinates between -1 and 1
		Shape* CreateShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout = {3});

		//! @brief creates a Shape based on coordinates between pixels, origin is at the bottom left
		//! @param VertexLayout pass a value of type "enum C2NDC" or a positive int value
		Shape* CreateShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<int> VertexLayout = {NDC_XYZ});

		void SetDefaultShader(Shader* shader);
		//! @brief draws the shape
		//! @param DrawFunc Performs this function between binding the shader and drawing the Shape
		void Draw(Shape* Shape, void (*DrawFunc)(void) = nullptr);

		Camera* GetCamera(void);
		Shader* GetGlobalShader(void);
};

} // namespace
#endif