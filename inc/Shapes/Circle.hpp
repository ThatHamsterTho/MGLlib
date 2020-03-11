#ifndef CIRCLEHEADERGUARD
#define CIRCLEHEADERGUARD

#include "Shape.h"

namespace MGLlib
{
	class Circle : public Shape {
		float Mx, My;
		float Radius;

		public:
		Circle(Shader* shader, Usage_Type uType = Draw_Dynamic);
		void MakeShape(float Mx, float My, float Z, float Radius, int Sectors);
		~Circle();

		float getRadius(void){return Radius;};
	};
} // namespace MGLlib


#endif