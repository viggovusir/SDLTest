#include "Vector2.h"
#include <cmath>

Vector2::Vector2() {
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2& Vector2::Add(const Vector2& vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2& Vector2::Subtract(const Vector2& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2& Vector2::Divide(const float divisor) {
	this->x /= divisor;
	this->y /= divisor;
	
	return *this;
}

Vector2& Vector2::Divide(const Vector2& divisor) {
	this->x /= divisor.x;
	this->y /= divisor.y;

	return *this;
}

Vector2& Vector2::Normalize() {
	float mag = this->Magnitude();
	if (mag != 0.0f) {
		return this->Divide(mag);
	}
	return *this;
}



float Vector2::Magnitude() {
	return std::sqrt((this->x * this->x) + (this->y * this->y));
}

Vector2& operator+(Vector2& vec1, const Vector2& vec2) {
	return vec1.Add(vec2);
}

Vector2& operator-(Vector2& vec1, const Vector2& vec2) {
	return vec1.Subtract(vec2);
}