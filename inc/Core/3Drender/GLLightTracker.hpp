#ifndef GLLIGHTRACKERHEADERGUARD
#define GLLIGHTRACKERHEADERGUARD

#include "GLTypeTracker.hpp"

namespace MGLlib
{

class GLLightTracker : public GLTypeTracker
{
public:
	// keeps track of GL light data
	// for more info on light types: https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glLight.xml
	GLLightTracker(unsigned int GLlight);
 	~GLLightTracker();
	void enable(void){glEnable(this->GLnum);}
};

} // namespace MGLlib

#endif