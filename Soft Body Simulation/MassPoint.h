#pragma once
#include "SoftBody.h"
using namespace sf;

class Spring;
class SoftBody;
class FixedBody;

class MassPoint
{
public:
	MassPoint(SoftBody* body, Vector2f pos);

	void setPosition(Vector2f pos);
	void setVelocity(Vector2f vel);
	void setMass(float mass);

	void lock(Vector2f lockPosition);
	void unlock();
	void highlight();

	Vector2f getPosition();
	Vector2f getVelocity();

	void addSpring(Spring* spring);

	void addForce(Vector2f force);
	void update(float time, bool log);
	void print(RenderWindow& window);

protected:

	SoftBody* body;
	std::vector<Spring*> spring;

	Vector2f position;
	Vector2f velocity;
	Vector2f force;
	float mass;

	bool locked;
	bool doHighlight;

	void manageCollision(float time, FixedBody& fixedbody, bool log);

};