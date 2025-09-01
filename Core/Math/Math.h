#pragma once
#include <cmath>

using namespace std;

class RedactedMath {
public:
	static float LerpF(float value, float target, float smoothing) {
		if (value > target)
			return value + (target - value) * smoothing;
		else
			return target + (value - target) * smoothing;
	}

	static float LerpF(float value, float target, float smoothing, float deltaTime) {
			return LerpF(value, target, 1 - std::pow(smoothing, deltaTime));
	}
};