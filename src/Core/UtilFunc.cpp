#include "UtilFunc.hpp"  
#include <stdlib.h>
#include <iostream>

namespace MGLlib { 
namespace Util {

float toNDC(double in, double max){
	return in/max;
}

float toPixel(double in, double max){
	return in * max;
}

// https://codegolf.stackexchange.com/questions/85555/the-fastest-square-root-calculator
float fastsquare(float n) {
    n = 1.0f / n;
    long i;
    float x, y;

    x = n * 0.5f;
    y = n;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (x * y * y));

    return y;
}

void quickrandomsetseed(void){
    srand(time(NULL));
}

int GetQuickRandom(int min, int max){
    return rand() % max + min;
}

// random generator
RandomGenerator::RandomGenerator(){
	std::random_device rd;	// slower rng but does not need to be seeded
	std::mt19937 mt(rd());	// seed the faster rng with the random_device class
	this->rng = mt;			// set the class' rng to the seeded rng
}

RandomGenerator::~RandomGenerator() {}

// returns value in range [min, max), thus max can never be reached.
template <typename T>
T RandomGenerator::GetRandom(T min, T max) {
	std::uniform_real_distribution<T> dist(min, max);
	return dist(rng);
}

}} // namespace