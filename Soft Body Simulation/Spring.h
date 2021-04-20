#pragma once
#include "MassPoint.h"
using namespace sf;
#pragma once

class MassPoint;
class SoftBody;

class Spring
{
public:

	Spring(MassPoint* A, MassPoint* B);

	MassPoint* getConnection(MassPoint* masspoint);
	Vector2f getForce(MassPoint* masspoint);

	void update();
	void print(RenderWindow& window);

protected:
	MassPoint* A;
	MassPoint* B;
	float stiffness;
	float originalLength;
	float damping;

	float springForce;
	float dampingForce;
	float currentLength;

	Vector2f forceA, forceB;

};

