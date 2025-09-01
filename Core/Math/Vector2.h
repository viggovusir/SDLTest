#pragma once

class Vector2 {
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	Vector2& Add(const Vector2& vec);

	Vector2& Subtract(const Vector2& vec);

	Vector2& Divide(const float divisor);
	Vector2& Divide(const Vector2& divisor);

	Vector2& Normalize();

	float Magnitude();

	static Vector2& Zero() {
		Vector2 res = { 0,0 };
		return res;
	}

	static Vector2& Divide(const Vector2& vec, const float divisor) {
		float x = vec.x / divisor;
		float y = vec.y / divisor;
		Vector2 res = { 0,0 };
		res.x = x;
		res.y = y;
		return res;
	}

	static Vector2& NormalBetween(const Vector2& vec1, const Vector2& vec2) {
		Vector2 vec = { -(vec1.y - vec2.y), (vec1.x - vec2.x) };
		return vec.Normalize();
	}


	static float Dot(const Vector2& vec1, const Vector2& vec2) {
		return ((vec1.x * vec2.x) + (vec1.y * vec2.y));
	}

	friend Vector2& operator+(Vector2& vec1, const Vector2& vec2);
	Vector2& operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;

		return *this;
	}

	Vector2& operator+=(const float& other) {
		this->x += other;
		this->y += other;

		return *this;
	}

	friend Vector2& operator-(Vector2& vec1, const Vector2& vec2);
	Vector2& operator-=(const float& other) {
		this->x -= other;
		this->y -= other;

		return *this;
	}

	Vector2& operator*(const float& factor) {
		this->x *= factor;
		this->y *= factor;
		return *this;
	}
};