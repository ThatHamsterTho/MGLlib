#include "GenericShape.h"

namespace MGLlib {
    GenericShape::GenericShape(){}
    GenericShape::~GenericShape(){}

    const std::pair<ShapeType, std::vector<unsigned int>> GenericShape::IndexBufferMap[SHAPETYPECOUNT] = {
        {POINTS, {0}},
        {TRIANGLES, {0, 1, 2}},
        {QUADS, {0, 1, 2, 2, 3, 0}},
        {POLYGON, {}}
    };

}

