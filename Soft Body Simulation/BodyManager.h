#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BodyCollection.h"

class BodyCollection;

class BodyManager
{
public:
	BodyManager();
	void add(SoftBody& body);
	void add(FixedBody& body);

	Vector2f getCollisionForces(MassPoint& masspoint);

	void print(RenderWindow& window);
	void update(float time);
	MassPoint* getMassPoint(Vector2f position, float distance);

private:
	BodyCollection collection;
};

