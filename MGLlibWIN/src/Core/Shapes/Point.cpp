#include "Core/Primitives/Point.hpp"

namespace MGLlib {
namespace Primitives {

Point::Point(){
	resetPoint();
}

Point::Point(CoordinateType Type, float x, float y, float z){
	if(Type == type_NDC){
		this->x(x);
		this->y(y);
		this->z(z);
	}
	else{
		this->nx(x);
		this->ny(y);
		this->nz(z);
	}
}

void Point::SetDimension(float width, float height, float depth){
	_width = width; 
	_height = height; 
	_depth = depth;
}

void Point::resetPoint(void){
	for(int i = 0; i < 3; i++){
		vertice.v[i] = 0.0;
		norm.v[i] = 0.0;
	}
}

Point::~Point(){}

// setter x
void Point::x(float x) { 
	vertice.x = x; 
	_lastupdated = U_non_norm;
}
// getter x
const float& Point::x() { 
	Update(U_non_norm);
	return vertice.x; 
}

// setter y
void Point::y(float y) { 
	vertice.y = y; 
	_lastupdated = U_non_norm;
}
// getter y
const float& Point::y() { 
	Update(U_non_norm);
	return vertice.y; 
}

// setter nz
void Point::z(float z){
	vertice.z = z;
	_lastupdated = U_non_norm;
}
// getter nz
const float& Point::z() {
	Update(U_non_norm);
	return vertice.z;
}


// setter position array
void Point::pos(const float pos[3]) {
	vertice.v[0] = pos[0]; 
	vertice.v[1] = pos[1];
	vertice.v[2] = pos[2];
	_lastupdated = U_non_norm;
}
// getter position array
const float* Point::pos() { 
	Update(U_non_norm);
	return vertice.v; 
}


// setter normalized x
void Point::nx(float nx) { 
	norm.x = nx; 
	_lastupdated = U_norm;
}
// getter normalized x
const float& Point::nx() { 
	Update(U_norm);
	return norm.x; 
}


// setter normalized y
void Point::ny(float ny) { 
	norm.y = ny; 
	_lastupdated = U_norm;
}
// getter normalized y
const float& Point::ny() { 
	Update(U_norm);
	return norm.y; 
}


// setter normalized z
void Point::nz(float nz) {
	norm.z = nz;
	_lastupdated = U_norm;
}

const float& Point::nz(){
	Update(U_norm);
	return norm.z;
}

// setter normalized position array
void Point::npos(const float npos[3]) {
	norm.v[0] = npos[0]; 
	norm.v[1] = npos[1];
	norm.v[2] = npos[2];
	_lastupdated = U_norm;
}
// getter normalized position array
const float* Point::npos() { 
	Update(U_norm);
	return norm.v; 
}

// modifiy position
void Point::modpos(float modx, float mody, float modz) { 
	x(vertice.x + modx); y(vertice.y + mody); z(vertice.z + modz); 
	}
void Point::modx(float modx) { x(vertice.x + modx); }
void Point::mody(float mody) { y(vertice.y + mody); }
void Point::modz(float modz) { z(vertice.z + modz); }
// modify normalized position
void Point::modnpos(float modx, float mody, float modz) { 
	nx(norm.x + modx); ny(norm.y + mody); nz(norm.z + modz);
	}
void Point::modnx(float modx) { nx(norm.x + modx); }
void Point::modny(float mody) { ny(norm.y + mody); }
void Point::modnz(float modz) { nz(norm.z + modz); }

// update normalized or non-normalized only if necessary
void Point::Update(__updated__ gettertype){
	// there was an update and the getter wants the non-updated type
	if(_lastupdated && (_lastupdated != gettertype)){
		// want: norm; lastupdated = non-norm
		if(gettertype && (_lastupdated == U_non_norm)){ 
			norm.x = Util::toNDC(vertice.x, _width);
			norm.y = Util::toNDC(vertice.y, _height);
			norm.z = Util::toNDC(vertice.z, _depth);
			_lastupdated = U_none;
		} 
		// want: non-norm; lastupdated = non-norm
		else if(!gettertype && (_lastupdated == U_norm)){
			vertice.x = Util::toPixel(norm.x, _width);
			vertice.y = Util::toPixel(norm.y, _height);
			vertice.z = Util::toPixel(norm.z, _depth);
			_lastupdated = U_none;
		}
	}
}

Point* Point::operator -(Point* A){
	Point *B = new Point();
	B->nx(this->nx() - A->nx());
	B->ny(this->ny() - A->ny());
	B->nz(this->nz() - A->nz());
	return B;
}

Point* Point::operator +(Point* A){
	Point *B = new Point();
	B->nx(this->nx() + A->nx());
	B->ny(this->ny() + A->ny());
	B->nz(this->nz() + A->nz());
	return B;
}

Point* Point::operator *(Point* A){
	Point *B = new Point();
	B->nx(	 this->ny()*A->nz() - this->nz()*A->ny());
	B->ny(- (this->nx()*A->nz() - this->nz()*A->nx()));
	B->nz(	 this->nx()*A->ny() - this->ny()*A->nx());
	return B;
}

Point* Point::operator /(Point* A){
	Point *B = new Point();
	B->nx(this->nx() / A->nx());
	B->ny(this->ny() / A->ny());
	B->nz(this->nz() / A->nz());
	return B;
}

float Point::_width = 1.0f;
float Point::_height = 1.0f;
float Point::_depth = 1.0f;

}}	// namespace
