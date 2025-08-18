#pragma once
#include <vector>
#include "../Core/Math/Vector2.h"
#include <SDL3/sdl.h>

using namespace std;

struct Transform {
	Vector2 position;
	Vector2 scale;
	Vector2 localOrigin = {0,0};
	float rotation;

	SDL_FRect rect() {
		SDL_FRect res = { 0,0,0,0 };
		res.x = position.x;
		res.y = position.y;
		res.w = scale.x;
		res.h = scale.y;
		return res;
	}
};

struct Sprite {
	int r;
	int g;
	int b;
	int a;
	int layerDepth;
};

struct Position {
	int x = 0;
	int y = 0;
};

struct Player {
	float moveSpeed;
};

struct CameraTarget {
	Transform position;
};

struct Ball {
	float moveSpeed = 2;
};

struct RigidBody {
	Vector2 velocity;
	float mass;
};

struct BoundingBox {
	float x;
	float y;
	float w;
	float h;
};

struct SATBoundingBox {
	std::vector<Vector2> vertices;
	std::vector<Vector2> edges;
	std::vector<Vector2> normals;

	void Update() {
		// MAKE DYNAMIC LATER
		edges[0] = vertices[0] - vertices[1];
		edges[1] = vertices[1] - vertices[2];
		edges[2] = vertices[2] - vertices[3];
		edges[3] = vertices[3] - vertices[0];

		for (int i = 0; i < normals.size(); i++) {
			normals[i].x = edges[i].y * -1;
			normals[i].y = edges[i].x;
		}
	}
};