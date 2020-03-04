#ifndef DEBUGGERHEADERGUARD
#define DEBUGGERHEADERGUARD

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <cassert>

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))
#define GLCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

void GLClearError();
void GLLogCall(const char* function, const char* file, int line);

#endif