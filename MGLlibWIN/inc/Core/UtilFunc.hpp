#ifndef UTILFUNCHEADERGUARD
#define UTILFUNCHEADERGUARD

#include <random>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace MGLlib { 
namespace Util {

float toNDC(float in, float max);
float toPixel(float in, float max);
float fastsquare(float n);

//! @brief returns a heap pointer to an array which contents match the vector's
template<typename type>
type* VecToArray(std::vector<type> vec){
	type* array = new type[vec.size()];
	for(unsigned int i = 0; i < vec.size(); i++){
		array[i] = vec[i];
	}
	return array;
}

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