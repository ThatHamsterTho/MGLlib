#include "Debugger.hpp"

#include <GL/gl.h>
#include <iostream>


void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}

void GLLogCall(const char* function, const char* file, int line){
	if(GLenum error = glGetError()){
		printf("[OpenGL Error] ( 0x%X ): on %s in %s : %d\n", error, function, file, line);
		while(true){};
	}
}

