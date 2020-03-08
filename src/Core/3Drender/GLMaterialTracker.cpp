#include "GLMaterialTracker.hpp"

namespace MGLlib
{

GLMaterialTracker::GLMaterialTracker(unsigned int GLmaterial) : GLTypeTracker(GLmaterial, glMaterialfv){
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_AMBIENT, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_DIFFUSE, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SPECULAR, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_EMISSION, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SHININESS, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_AMBIENT_AND_DIFFUSE, {8, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_COLOR_INDEXES, {8, 3}));
}

GLMaterialTracker::~GLMaterialTracker(){}

} // namespace MGLlib