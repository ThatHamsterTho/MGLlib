#ifndef POINTHEADERGUARD
#define POINTHEADERGUARD

#include "Core/UtilFunc.hpp"

namespace MGLlib {
namespace Primitives {

typedef union __union_point__ {
	struct {
		float x, y, z;
	};
	float v[3];
} union_point;

enum CoordinateType{
	type_NDC,
	type_Pixel
};

class Point
{
	private:
		union_point vertice;	// non-normalized coordinates
		union_point norm;		// normalized coordinates <0-1>

		static float _width;
		static float _height;
		static float _depth;

		enum __updated__ {
			U_none,
			U_norm,
			U_non_norm
		} _lastupdated;
		void Update(__updated__ gettertype);
		
		// TODO: check if tonormalized needs to be called

	public:
		Point();
		Point(CoordinateType Type, float x, float y, float z = 0.0f);

		void SetDimension(float width, float height, float depth);
		void resetPoint(void);

		~Point();
		void x(float x);
		const float& x();
		
		void y(float y);
		const float& y();

		void z(float z);
		const float& z();

		void pos(const float pos[3]);
		const float* pos();

		// normalized x
		void nx(float nx);
		const float& nx();

		// normalized y
		void ny(float ny);
		const float& ny();
		void nz(float nz);
		const float& nz();

		// normalized position
		void npos(const float npos[3]);
		const float* npos();

		void modpos(float modx, float mody, float modz);
		void modx(float modx);
		void mody(float mody);
		void modz(float modz);

		void modnpos(float modx, float mody, float modz);
		void modnx(float modx);
		void modny(float mody);
		void modnz(float modz);

		Point* operator -(Point* A);
		Point* operator +(Point* A);
		Point* operator *(Point* A);
		Point* operator /(Point* A);
};

}}	// namespace

#endif