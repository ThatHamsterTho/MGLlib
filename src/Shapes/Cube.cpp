#include "Cube.hpp"

namespace MGLlib
{
	Cube::Cube(Shader* shader, Usage_Type uType) : Shape(MGL_QUADS, shader, uType){		
		// cube sides
		//float normals[6][3] = {
		//	{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
		//	{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
		//};
	}
	Cube::~Cube(){}


	void Cube::SetCube(float x, float y, float z, float width, float height, float depth){
		float c[8][3] = {
			{x		, y			, z+depth},	// 0
			{x		, y			, z		 },	// 1
			{x		, y+height	, z		 },	// 2
			{x		, y+height	, z+depth},	// 3
			{x+width, y			, z+depth},	// 4
			{x+width, y			, z		 },	// 5
			{x+width, y+height	, z		 },	// 6
			{x+width, y+height	, z+depth}	// 7
		};
		SetCube(c);
	}

	void Cube::SetCube(float x0, float y0, float z0, 
						float x1, float y1, float z1, 
						float x2, float y2, float z2,
						float x3, float y3, float z3,
						float x4, float y4, float z4,
						float x5, float y5, float z5,
						float x6, float y6, float z6,
						float x7, float y7, float z7){
		float c[8][3] = {
			{x0, y0, z0},
			{x1, y1, z1},
			{x2, y2, z2},
			{x3, y3, z3},
			{x4, y4, z4},
			{x5, y5, z5},
			{x6, y6, z6},
			{x7, y7, z7}
		};	
		SetCube(c);
	}

	void Cube::SetCube(Cube *C){
		std::vector<std::array<float, 3>> CSides = C->GetModel();
		float c[8][3] = {
			CSides[0][0], CSides[0][1], CSides[0][2],
			CSides[1][0], CSides[1][1], CSides[1][2],
			CSides[2][0], CSides[2][1], CSides[2][2],
			CSides[3][0], CSides[3][1], CSides[3][2],
			CSides[8][0], CSides[8][1], CSides[8][2],
			CSides[9][0], CSides[9][1], CSides[9][2],
			CSides[10][0], CSides[10][1], CSides[10][2],
			CSides[11][0], CSides[11][1], CSides[11][2],
		};
		SetCube(c);
	}

	void Cube::SetCube(float c[8][3]){
		int map[6][4] = {
			{0, 1, 2, 3},
			{1, 5, 6, 2},
			{5, 4, 7, 6},
			{4, 0, 3, 7},
			{4, 0, 1, 5},
			{7, 3, 2, 6}
		};

		for(int j = 0; j < 6; j++){
			for(int i = 0; i < 4; i++){
				int vertex = map[j][i];
				SetVertex3D(j*6+i, {c[vertex][0], c[vertex][1], c[vertex][2]});
			}
		}	
	}

	void Cube::SetSideColor(int i, unsigned char R, unsigned char G, unsigned char B, unsigned char A){
		for(int j = 0; j < 4; j++){
			SetColorVec(4*i + j, {(float)R, (float)G, (float)B, (float)A});
		}
	}
} // namespace MGLlib
