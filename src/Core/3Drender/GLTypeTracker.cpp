#include "GLTypeTracker.hpp"

namespace MGLlib
{

GLTypeTracker::GLTypeTracker(unsigned int GLnum, GLsetparamfv paramsetfunc){
	this->GLnum = GLnum;
	this->paramsetfunc = paramsetfunc;
}

GLTypeTracker::~GLTypeTracker(){
	for(int i = 0; i < static_cast<int>(malloced.size()); i++){
		if(malloced[i]){
			free(parameters[i]);
		}
	}
}

void GLTypeTracker::GenAddParam(unsigned int GLtype, float *param, bool malloced){
	this->parameters.push_back(param);
	this->GLType_IndexMap.at(GLtype)[0] = static_cast<int>(this->GLTypeList.size());
	this->GLTypeList.push_back(GLtype);
	this->malloced.push_back(malloced);
}

void GLTypeTracker::addParameters(unsigned int GLtype, float param0, float param1, float param2, float param3){
	int currentparamlength = GLType_IndexMap.at(GLtype)[1];
	
	float *params = (float *)malloc(sizeof(float)*currentparamlength);
	float givenparams[] = {param0, param1, param2, param3};
	
	for(int i = 0; i < currentparamlength; i++){
		params[i] = givenparams[i];
	}

	this->GenAddParam(GLtype, params, true);
}

void GLTypeTracker::addParameters(unsigned int GLLightType, float *param){
	this->GenAddParam(GLLightType, param, false);
}

void GLTypeTracker::addRGBA(unsigned int GLLightType, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	float *params = (float *)malloc(sizeof(float)*4);
	params[0] = Util::toNDC(r, 255);
	params[1] = Util::toNDC(g, 255);
	params[2] = Util::toNDC(b, 255);
	params[3] = Util::toNDC(a, 255);
	this->GenAddParam(GLLightType, params, true);
}

void GLTypeTracker::setParameters(unsigned int GLtype, float *param){
	int index = GLType_IndexMap.at(GLtype)[0];
	if(index == static_cast<int>(this->GLType_IndexMap.size())+1){
		addParameters(GLtype, param);
	}
	else{
		if(this->malloced[index]){
			free(this->parameters[index]);
			this->malloced[index] = false;
		}
		this->parameters[index] = param;
	}
	Update(index);
}

void GLTypeTracker::setParameters(unsigned int GLtype, float param0, float param1, float param2, float param3){
	int index = GLType_IndexMap.at(GLtype)[0];
	if(index == static_cast<int>(this->GLType_IndexMap.size())+1){
		addParameters(GLtype, param0, param1, param2, param3);
	}
	else{
		if(this->malloced[index]){
			free(this->parameters[index]);
			this->malloced[index] = false;
		}
		
		int currentparamlength = GLType_IndexMap.at(GLtype)[1];
		
		float *params = (float *)malloc(sizeof(float)*currentparamlength);
		float givenparams[] = {param0, param1, param2, param3};
		
		for(int i = 0; i < currentparamlength; i++){
			params[i] = givenparams[i];
		}

		this->parameters[index] = params;
		this->malloced[index] = true;
	}
	Update(index);
}

// non-normalized RGBA values
void GLTypeTracker::setRGBA(unsigned int GLtype, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	int index = (GLType_IndexMap.at(GLtype))[0];
	if(index == static_cast<int>(this->GLType_IndexMap.size())+1){
		addRGBA(GLtype, r, g, b, a);
	}
	else{
		if(this->malloced[index]){
			free(this->parameters[index]);
			this->malloced[index] = false;
		}

		float *params = (float *)malloc(sizeof(float)*4);
		params[0] = Util::toNDC(r, 255);
		params[1] = Util::toNDC(g, 255);
		params[2] = Util::toNDC(b, 255);
		params[3] = Util::toNDC(a, 255);

		this->parameters[index] = params;
		this->malloced[index] = true;
	}
	Update(index);
}

void GLTypeTracker::Update(int typeindex){
	paramsetfunc(this->GLnum, this->GLTypeList[typeindex], this->parameters[typeindex]);
}

void GLTypeTracker::enableParameters(void){
	for(int i = 0; i < static_cast<int>(this->GLTypeList.size()); i++){
		paramsetfunc(this->GLnum, this->GLTypeList[i], this->parameters[i]);
	}
}

void GLTypeTracker::clone(GLTypeTracker* O){
	for(int i = 0; i < static_cast<int>(O->GLTypeList.size()); i++){
		if(i < static_cast<int>(this->GLTypeList.size())){
			this->GLTypeList[i] = O->GLTypeList[i];
			for(int j = 0; j < GLType_IndexMap.at(O->GLTypeList[i])[1]; j++){
				this->parameters[i][j] = O->parameters[i][j]; 
			}
		}
		else{
			addParameters(O->GLTypeList[i], O->parameters[i]);
		}
	}
}

} // namespace MGLlib