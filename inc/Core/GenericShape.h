#ifndef GENERICSHAPEHEADER
#define GENERICSHAPEHEADER

#include <vector>
#include <array>

namespace MGLlib{

	enum ShapeType {
		POINTS,
		LINES,
		LINE_STRIP,
		LINE_LOOP,
		POLYGON,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		QUADS,
		QUAD_STRIP,

		// keep this as last type
		SHAPETYPECOUNT
	};


class GenericShape
{
	static const std::pair<ShapeType, std::vector<unsigned int>> IndexBufferMap[SHAPETYPECOUNT];
	std::vector<unsigned int> indexBuffer;

	public:
		GenericShape();
		~GenericShape();

	private:


};

}	// namespace

#endif