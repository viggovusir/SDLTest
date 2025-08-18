#include "Core/Math/Vector2.h"
#include <vector>
static class CollisionManager {
public:
	static bool CheckSATOverlap(const std::vector<Vector2>& vertices1, const std::vector<Vector2>& vertices2);
	static bool EnteredCollider(const std::vector<Vector2>& collidingObject, const std::vector<Vector2>& collider);
	static Vector2& Projection(const std::vector<Vector2>& vertices, const Vector2& axis);
	static Vector2& MinimumVertexX(const std::vector<Vector2>& vertices);
	static Vector2& MinimumVertexY(const std::vector<Vector2>& vertices);
	static Vector2& MaximumVertexX(const std::vector<Vector2>& vertices);
	static Vector2& MaximumVertexY(const std::vector<Vector2>& vertices);
	static Vector2& MinimumTranslationVector(const std::vector<Vector2>& collidingObject, std::vector<Vector2>& collider);

private:
	static float CollisionResponseAcrossLine(const Vector2& vec1, const Vector2& vec2);
};