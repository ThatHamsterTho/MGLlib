#ifndef UTILFUNCHEADERGUARD
#define UTILFUNCHEADERGUARD

#include <random>
#include <cstdlib>
#include <ctime>

namespace MGLlib { 
namespace Util {

float toNDC(double in, double max);
float toPixel(double in, double max);
float fastsquare(float n);

// normalized color struct
typedef union __RGBA__ {
	struct {
		float r, g, b, a;
	};
	float *c;
} RGBA;

void quickrandomsetseed(void);
int GetQuickRandom(int min, int max); 

class RandomGenerator {
	private:
		std::mt19937 rng;
		uint32_t seed_val;

	public:
		RandomGenerator();
		~RandomGenerator();

		template <typename T>
		T GetRandom(T min, T max);
};


}} // namespace
#endif