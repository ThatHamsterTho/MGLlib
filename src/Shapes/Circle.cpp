#include "Circle.hpp"

namespace MGLlib
{
	Circle::Circle(Shader* shader, Usage_Type uType) : Shape(MGL_TRIANGLE_FAN, shader, uType){}
	Circle::~Circle(){};
	// https://stackoverflow.com/q/22444450
	void Circle::MakeShape(float Mx, float My, float Z, float Radius, int Sectors){
		this->Mx = Mx;
		this->My = My;
		if(Sectors < 3){
			pWARNING("not enough Vertices to make a circle minimum is 3");
		}
		if(!((Sectors-3)%2)){
			pWARNING("Sectors should have follow the formula: 3+2x, where X can be any value");
		}
		for(int i = 0; i <= Sectors; i++)
		{
			float theta = 2.0f * 3.14f * float(i) / float(Sectors);//get the current angle

			float x = Radius * cosf(theta);//calculate the x component
			float y = Radius * sinf(theta);//calculate the y component
			SetVertex3D(i, {
				x + Mx,
				y + My,
				Z
			});
		}
	}
	
} // namespace MGLlib
