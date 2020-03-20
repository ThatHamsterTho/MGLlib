#ifndef RECTANGLEHEADERGUARD
#define RECTANGLEHEADERGUARD

#include "Core/Shapes/Shape.h"

namespace MGLlib
{
	class Rectangle : public Shape
	{
		float width, height, depth;

		public:
			Rectangle(Shader* shader, Usage_Type uType = Draw_Dynamic);
			void MakeShape(float width, float height, float depth, bool tild = false);
			~Rectangle();
			float getWidth(void){return width;}
			float getHeight(void){return height;}
			float getDepth(void){return depth;}
	};

} // namespace MGLlib


#endif