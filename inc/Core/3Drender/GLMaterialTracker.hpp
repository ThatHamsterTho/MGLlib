#ifndef GLMATERIALTRACKERHEADERGUARD
#define GLMATERIALTRACKERHEADERGUARD

#include "GLTypeTracker.hpp"

namespace MGLlib {

class GLMaterialTracker : public GLTypeTracker
{
public:
	// keeps track of GL light data
	// for more info on light types: https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glLight.xml
	GLMaterialTracker(unsigned int GLmaterial);

 	~GLMaterialTracker();
};

} // namespace MGLlib
#endif
