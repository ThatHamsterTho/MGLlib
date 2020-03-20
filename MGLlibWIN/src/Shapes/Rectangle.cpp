#include "Shapes/Rectangle.hpp"

namespace MGLlib
{	
	void Rectangle::MakeShape(float width, float height, float depth, bool tild){
		this->width = width;
		this->height = height;
		this->depth = depth;
		bool graycode[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}}; 
		SetModel3D({
			0.0f , 0.0f,   0.0f,
			width, 0.0f,   graycode[1][!tild] ? depth : 0,
			width, height, graycode[2][!tild] ? depth : 0,
			0.0f , height, graycode[3][!tild] ? depth : 0
		});
		/*
		for(int i = 3; i >= 0; i--){
			SetVertex3D(i, {
				(graycode[i][0] ? v[0] : 0),
				(graycode[i][1] ? v[1] : 0),
				(graycode[i][!tild] ? v[2] : 0)
			});
		}
		*/
	}
	Rectangle::Rectangle(Shader* shader, Usage_Type uType) : Shape(MGL_QUADS, shader, uType){}
	Rectangle::~Rectangle(){};
} // namespace MGLlib
