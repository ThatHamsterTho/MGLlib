#ifndef CIRCLEHEADERGUARD
#define CIRCLEHEADERGUARD

#include "Shape.h"

namespace MGLlib
{
	class Circle : public Shape {
		float Mx, My;
		
		public:
		Circle(Shader* shader);
		void MakeShape(float Mx, float My, float Z, float Radius, int Sectors);
		~Circle();
	};
} // namespace MGLlib


#endif