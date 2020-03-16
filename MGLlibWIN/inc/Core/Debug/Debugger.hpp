#ifndef DEBUGGERHEADERGUARD
#define DEBUGGERHEADERGUARD

#include "DefineDebug.h"

#include <GL/glew.h>
#include <cassert>

namespace MGLlib { 

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))
#define GLCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

void GLClearError();
void GLLogCall(const char* function, const char* file, int line);

} // namespace
#endif