#include "Rope.h"
#include "Core/Game.h"
#include <iostream>
#include "Core/Input.h"

Rope::Rope() {
	ropeWidth = 1;
	numRopeSegments = 2;
	segmentLength = 5;
	gravityForce = { 0, 2 };
	dampingFactor = 0.98f;

	for (int i = 0; i < numRopeSegments; i++) {
		ropeSegments.push_back(RopeSegment({ 50, 50 + (i * segmentLength) }));
	}
}

Rope::Rope(int _ropeWidth, int _numSeg, float segLen, Vector2 grav, float damp, Vector2 startPos) {
	ropeWidth = _ropeWidth;
	numRopeSegments = _numSeg;
	segmentLength = segLen;
	gravityForce = grav;
	dampingFactor = damp;

	for (int i = 0; i < numRopeSegments; i++) {
		ropeSegments.push_back(RopeSegment({ startPos.x, startPos.y /*+ (i * segmentLength) */ }));
	}
}

void Rope::Update() {
	for (int i = 0; i < ropeSegments.size(); i++) {
		RopeSegment seg = ropeSegments[i];

		if (i != 0) {

			Vector2 velocity = (seg.currentPos);
			velocity.x = (seg.currentPos.x - seg.previousPos.x) * dampingFactor;
			velocity.y = (seg.currentPos.y - seg.previousPos.y) * dampingFactor;
			seg.previousPos = seg.currentPos;
			seg.currentPos.Add(velocity);
			seg.currentPos += ((gravityForce) * Game::DeltaTime);

		}

		ropeSegments[i] = seg;

		//std::cout << "Segment\n";
	}
	for (int i = 0; i < 50; i++)
	ApplyConstraint();
}

void Rope::Draw(SDL_Renderer*& renderer) {
	for (int i = 0; i < ropeSegments.size(); i++) {
		if (i == 0)
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderPoint(renderer, ropeSegments[i].currentPos.x, ropeSegments[i].currentPos.y);
	}
}

void Rope::ApplyConstraint() {
	RopeSegment firstSegment = ropeSegments[0];
	//firstSegment.currentPos.x += 5 * Game::DeltaTime;
	firstSegment.currentPos.x == -50;
	firstSegment.currentPos.y == -50;

	//if (Input::keyPressed(SDL_SCANCODE_F))
	//	firstSegment.currentPos.x == 0;

	ropeSegments[0] = firstSegment;

	for (int i =0; i < ropeSegments.size() - 1; i++) {
		RopeSegment currentSeg = ropeSegments[i];
		RopeSegment nextSeg = ropeSegments[i + 1];

		float dist = (currentSeg.currentPos - nextSeg.currentPos).Magnitude();
		float difference = (dist - segmentLength);

		Vector2 translationDirection = (currentSeg.currentPos - nextSeg.currentPos).Normalize();
		Vector2 translationVector = { translationDirection.x * difference, translationDirection.y * difference };

		//if (i != 0) {
		//	currentSeg.currentPos -= (translationVector * 0.5f);
		//	currentSeg.currentPos.x -= (translationVector.x * 0.5f);
		//	currentSeg.currentPos.y -= (translationVector.y * 0.5f);
		//	nextSeg.currentPos.x += (translationVector.x * 0.5f);
		//	nextSeg.currentPos.y += (translationVector.y * 0.5f);
		//}
		//else {
		//	nextSeg.currentPos += translationVector;
		//	//nextSeg.currentPos.x += translationVector.x * Game::DeltaTime;
		//	//nextSeg.currentPos.y += translationVector.y * Game::DeltaTime;
		//}

		ropeSegments[i] = currentSeg;
		ropeSegments[i + 1] = nextSeg;
	}
}