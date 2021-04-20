#include "BodyCollection.h"

BodyCollection::BodyCollection()
{
}

void BodyCollection::add(SoftBody& body)
{
	this->softbody.push_back(body);
}

void BodyCollection::add(FixedBody& body)
{
	this->fixedbody.push_back(body);
}

std::vector<FixedBody>* BodyCollection::getFixedBodies()
{
	return &fixedbody;
}

MassPoint* BodyCollection::getMassPoint(Vector2f position, float distance)
{
	MassPoint* result = nullptr;
	for (int i = 0; i < softbody.size(); ++i) {
		result = softbody[i].getMassPoint(position, distance);
		if (result != nullptr) return result;
	}
	return nullptr;
}

void BodyCollection::print(RenderWindow& window)
{
	for (int i = 0; i < fixedbody.size(); ++i) fixedbody[i].print(window);
	for (int i = 0; i < softbody.size(); ++i) softbody[i].print(window);
}

void BodyCollection::update(float time)
{
	for (int i = 0; i < softbody.size(); ++i) softbody[i].update(time);
}
