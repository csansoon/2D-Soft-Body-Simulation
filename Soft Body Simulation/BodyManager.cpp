#include "BodyManager.h"

BodyManager::BodyManager()
{
	this->collection = BodyCollection();
}

void BodyManager::add(SoftBody& body)
{
	collection.add(body);
	body.setBodyCollection(&collection);
}

void BodyManager::add(FixedBody& body)
{
	collection.add(body);
}

Vector2f BodyManager::getCollisionForces(MassPoint& masspoint)
{
	return Vector2f();
}

void BodyManager::print(RenderWindow& window)
{
	collection.print(window);
}

void BodyManager::update(float time)
{
	collection.update(time);
}

MassPoint* BodyManager::getMassPoint(Vector2f position, float distance)
{
	return collection.getMassPoint(position, distance);
}