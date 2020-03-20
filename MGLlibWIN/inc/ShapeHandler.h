#ifndef SHAPEHANDLER
#define SHAPEHANDLER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/3Drender/Camera.h"
#include "Core/Shapes/Shader.hpp"		// for default shader

#include "Core/Shapes/Shape.h"

#include "Shapes/Rectangle.hpp"
#include "Shapes/Circle.hpp"
#include "Shapes/Cube.hpp"

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

	
		ShapeHandler();
		~ShapeHandler();
		
		void InitHandler(GLFWwindow* window);

		static Shape* baseShape;
	public:
		void Enable3Drender(void);
		void Disable3Drender(void);
		void Enableortho(void);
		void Disableortho(void);

		void SetDefaultShader(Shader* shader);
		//! @brief draws the shape
		//! @param DrawFunc Performs this function between binding the shader and drawing the Shape
		void Draw(GenericShape* Shape, void (*DrawFunc)(void) = nullptr);
		void Draw(Shape* Shape, void (*DrawFunc)(void) = nullptr);

		Camera* GetCamera(void);
		Shader* GetGlobalShader(void);

		//! @brief creates a Shape based on coordinates between -1 and 1
		GenericShape* CreateGenShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout = {3});

		//! @brief creates a Shape based on coordinates between pixels, origin is at the bottom left
		//! @param VertexLayout pass a value of type "enum C2NDC" or a positive int value
		GenericShape* CreateGenShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<int> VertexLayout = {NDC_XYZ});

		//! @brief returns an empty shape object
		//! @remark shape data should be set after calling this function
		Shape* CreateShape(ShapeType ST, Usage_Type uType = Draw_Dynamic);

		//! @brief returns a rectangle object
		Rectangle* CreateRectangle(float width, float height, float depth, bool tild = false, Usage_Type uType = Draw_Dynamic);
		Rectangle* CreateRectangle(float width, float height, Usage_Type uType = Draw_Dynamic);
		Circle* CreateCircle(float x, float y, float z, float Radius, int Sectors, Usage_Type uType = Draw_Dynamic);
		Circle* CreateCircle(float x, float y, float Radius, int Sectors, Usage_Type uType = Draw_Dynamic);
		Cube* CreateCube(float x, float y, float z, float width, float height, float depth, Usage_Type uType = Draw_Dynamic);
};

} // namespace
#endif