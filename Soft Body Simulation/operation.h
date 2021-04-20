#pragma once
#include <SFML/System/Vector2.hpp>
#include <iostream>
using namespace sf;

class operation
{
public:

	struct intersection {
		float x;
		float y;
		bool intersects;
	};

	struct location {
		float x;
		float y;
		bool before;
		bool after;
	};


	static intersection segment_intersection(Vector2f A, Vector2f B, Vector2f C, Vector2f D);

	static location closestPoint(Vector2f P, Vector2f A, Vector2f B);

	static float distanceBetween(Vector2f A, Vector2f B);

	static Vector2f normalize(Vector2f vector);

	static float dotProduct(Vector2f A, Vector2f B);

	static std::string direction(Vector2f vector);
};

