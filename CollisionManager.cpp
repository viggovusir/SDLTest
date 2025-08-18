#include "CollisionManager.h"

bool CollisionManager::CheckSATOverlap(const std::vector<Vector2>& vertices1, const std::vector<Vector2>& vertices2) {
	for (int i = 0; i < vertices1.size(); i++) {
		Vector2 currentVertex = vertices1[i];
		Vector2 nextVertex = vertices1[(i + 1) % vertices1.size()];

		Vector2 normal = Vector2::NormalBetween(currentVertex, nextVertex);
		Vector2 projection1 = Projection(vertices1, normal);
		Vector2 projection2 = Projection(vertices2, normal);

		if (!(projection1.x <= projection2.y && projection1.y >= projection2.x))
			return false;
	}
	return true;
}

bool CollisionManager::EnteredCollider(const std::vector<Vector2>& collidingObject, const std::vector<Vector2>& collider) {
	bool hasEntered = false;
	if (!hasEntered && CheckSATOverlap(collidingObject, collider))
		hasEntered = true;

	return hasEntered;
}

Vector2& CollisionManager::Projection(const std::vector<Vector2>& vertices, const Vector2& axis) {
	Vector2 res = { 0,0 };

	float min = Vector2::Dot(axis, vertices[0]);
	float max = min;
	
	Vector2 _axis = axis;

	for (int i = 0; i < vertices.size(); i++) {
		_axis.Normalize();
		float p = (float)(Vector2::Dot(_axis, vertices[i]));
		if (p < min)
			min = p;
		else if (p > max)
			max = p;
	}
	res.x = min;
	res.y = max;
	return res;
}

Vector2& CollisionManager::MinimumVertexX(const std::vector<Vector2>& vertices) {
	Vector2 min = vertices[0];
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x < min.x)
			min = vertices[i];
	}
	return min;
}

Vector2& CollisionManager::MinimumVertexY(const std::vector<Vector2>& vertices) {
	Vector2 min = vertices[0];
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].y < min.y)
			min = vertices[i];
	}
	return min;
}

Vector2& CollisionManager::MaximumVertexX(const std::vector<Vector2>& vertices) {
	Vector2 max = vertices[0];
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x > max.x)
			max = vertices[i];
	}
	return max;
}

Vector2& CollisionManager::MaximumVertexY(const std::vector<Vector2>& vertices) {
	Vector2 max = vertices[0];
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].y > max.y)
			max = vertices[i];
	}
	return max;
}

Vector2& CollisionManager::MinimumTranslationVector(const std::vector<Vector2>& collidingObject, std::vector<Vector2>& collider) {
	if (!CheckSATOverlap(collidingObject, collider))
		return Vector2::Zero();

	float minimumResponseMagnitude = FLT_MAX;
	Vector2 responseNormal = Vector2::Zero();

	for (int i = 0; i < collider.size(); i++) {
		Vector2 currentVertex = collider[i];
		Vector2 nextVertex = collider[(i + 1) % collider.size()];

		Vector2 edgeNormal = Vector2::NormalBetween(currentVertex, nextVertex);

		Vector2 colliderProjected = Projection(collider, edgeNormal);
		Vector2 collidingProjected = Projection(collidingObject, edgeNormal);
		
		float responseMagnitude = CollisionResponseAcrossLine(colliderProjected, collidingProjected);

		if (responseMagnitude != NULL && std::abs(responseMagnitude) < std::abs(minimumResponseMagnitude)) {
			minimumResponseMagnitude = responseMagnitude;
			responseNormal = edgeNormal;
		}
	}

	Vector2 res = { 0,0 };
	res.x = responseNormal.x * minimumResponseMagnitude;
	res.y = responseNormal.y * minimumResponseMagnitude;
	return res;
}

float CollisionManager::CollisionResponseAcrossLine(const Vector2& vec1, const Vector2& vec2) {
	float distToVec2Start = vec2.x - vec1.y;
	if (distToVec2Start > 0)
		return NULL;

	float distToVec2End = vec2.y - vec1.x;
	if (distToVec2End < 0)
		return NULL;

	if (-distToVec2Start < distToVec2End)
		return distToVec2Start;
	else
		return distToVec2End;
}