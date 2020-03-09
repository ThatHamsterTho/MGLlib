#ifndef RECTANGLEHEADERGUARD
#define RECTANGLEHEADERGUARD

#include "Shape.h"

namespace MGLlib
{
	class Rectangle : public Shape
	{
		float width, height, depth;

		public:
			Rectangle(Shader* shader, Usage_Type uType = Draw_Dynamic);
			void MakeShape(float x, float y, float z, float width, float height, float depth, bool tild = false);
			~Rectangle();
	};

} // namespace MGLlib


#endif