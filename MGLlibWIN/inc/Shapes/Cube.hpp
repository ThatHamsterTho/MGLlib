#ifndef CUBEHEADERGUARD
#define CUBEHEADERGUARD

#include "Core/Shapes/Shape.h"

namespace MGLlib
{
	class Cube : public Shape{
	private:
		// list of vertices

	public:
		Cube(Shader* shader, Usage_Type uType = Draw_Dynamic);
		~Cube();

		// set the cube's material to given material
		//void SetMaterial(GLMaterialTracker *M);
		// set's the cube's material to side 0
		//void SetMaterialShape(void);
		// get side 0's material
		//GLMaterialTracker*& getMaterial(void);

		void SetCube(float x, float y, float z, float width, float height, float depth);
		void SetCube(float x0, float y0, float z0, 
					float x1, float y1, float z1, 
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float x4, float y4, float z4,
					float x5, float y5, float z5,
					float x6, float y6, float z6,
					float x7, float y7, float z7);
		void SetCube(float c[8][3]);
		void SetCube(Cube *C);
		//   5
		// 0 1 2 3
		//   4
		void SetSideColor(int i, unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255);
		//GLMaterialTracker*& getSideMaterial(int i);
		//void setSideMaterial(int i, GLMaterialTracker *M);
		Shape* getSides(void);
	};
	
} // namespace MGLlib


#endif