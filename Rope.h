#pragma once
#include <vector>
#include <SDL3/SDL_render.h>
#include "Core/Math/Vector2.h"

struct RopeSegment {
	RopeSegment(Vector2 startPos) {
		currentPos = startPos;
		previousPos = startPos;
	}

	Vector2 currentPos;
	Vector2 previousPos;
};

class Rope {
public:
	Rope();
	Rope(int _ropeWidth, int _numSeg, float segLen, Vector2 grav, float damp, Vector2 startPos);

	void Update();
	void Draw(SDL_Renderer*& renderer);

private:
	int ropeWidth;
	int numRopeSegments;
	float segmentLength;

	Vector2 gravityForce;
	float dampingFactor;

	std::vector<RopeSegment> ropeSegments;

	void ApplyConstraint();
};