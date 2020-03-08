#ifndef GLTYPETRACKERHEADERGUARD
#define GLTYPETRACKERHEADERGUARD

#include "UtilFunc.hpp"
#include <vector>
#include <map>

#include <GL/freeglut.h>
#include <GL/gl.h>

namespace MGLlib {

class GLTypeTracker{
private:
	typedef void (*GLsetparamfv)(GLenum num, GLenum type, const GLfloat* params);

	std::vector<unsigned int> GLTypeList;
	std::vector<float *> parameters;
	std::vector<bool> malloced;

	GLsetparamfv paramsetfunc;

	void GenAddParam(unsigned int GLtype, float *param, bool malloced);

	void addParameters(unsigned int GLtype, float *param);
	void addParameters(unsigned int GLtype, float param0, float param1, float param2, float param3);

	// non-normalized RGBA values
	void addRGBA(unsigned int GLLightType, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void Update(int typeindex);
	
protected:
	unsigned int GLnum;
	std::map<unsigned int, std::vector<int>> GLType_IndexMap;

public:
 	GLTypeTracker(unsigned int GLnum, GLsetparamfv paramsetfunc);
 	~GLTypeTracker();

	void setParameters(unsigned int GLLightType, float *param);
	void setParameters(unsigned int GLLightType, float param0 = -1.0, float param1 = -1.0, float param2 = -1.0, float param3 = -1.0);

	// non-normalized RGBA values
	void setRGBA(unsigned int GLLightType, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void clone(GLTypeTracker* O);
	void enableParameters(void);
};

} // namespace MGLlib
#endif
