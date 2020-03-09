#include "Rectangle.hpp"

namespace MGLlib
{	
	void Rectangle::MakeShape(float x, float y, float z, float width, float height, float depth, bool tild){
		this->width = width;
		this->height = height;
		this->depth = depth;
		float v[3] = { width, height, depth };
		bool graycode[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}}; 
		for(int i = 0; i < 4; i++){
			SetVertex3D(i, {
				x + (graycode[i][0] ? v[0] : 0),
				y - (graycode[i][1] ? v[1] : 0),
				z + (graycode[i][!tild] ? v[2] : 0)
			});
		}
	}
	Rectangle::Rectangle(Shader* shader, Usage_Type uType) : Shape(MGL_QUADS, shader, uType){}
	Rectangle::~Rectangle(){};
} // namespace MGLlib
