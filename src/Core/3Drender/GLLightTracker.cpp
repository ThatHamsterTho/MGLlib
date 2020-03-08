#include "GLLightTracker.hpp"

namespace MGLlib {

GLLightTracker::GLLightTracker(unsigned int GLlight) : GLTypeTracker(GLlight, glLightfv){
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_AMBIENT, {11, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_DIFFUSE, {11, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SPECULAR, {11, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_POSITION, {11, 4}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SPOT_DIRECTION, {11, 3}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SPOT_EXPONENT, {11, 1}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_SPOT_CUTOFF, {11, 1}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_CONSTANT_ATTENUATION, {11, 1}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_LINEAR_ATTENUATION, {11, 1}));
	this->GLType_IndexMap.insert(std::pair<unsigned int, std::vector<int>>(GL_QUADRATIC_ATTENUATION, {11, 1}));
}

GLLightTracker::~GLLightTracker(){}

} // namespace MGLlib