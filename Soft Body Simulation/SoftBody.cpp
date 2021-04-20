#include "SoftBody.h"
#include "operation.h"
using namespace sf;

SoftBody::SoftBody() {
	/* CUBE 
	massPoint.push_back(new MassPoint(this, Vector2f(-200, -200)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, -200)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, 200)));
	massPoint.push_back(new MassPoint(this, Vector2f(-200, 200)));

	linkPoints(massPoint[0], massPoint[1]);
	linkPoints(massPoint[0], massPoint[2]);
	linkPoints(massPoint[0], massPoint[3]);
	linkPoints(massPoint[3], massPoint[1]);
	linkPoints(massPoint[3], massPoint[2]);
	linkPoints(massPoint[1], massPoint[2]);

	massPoint[0]->setMass(10);
	massPoint[1]->setMass(10);
	massPoint[2]->setMass(10);
	massPoint[3]->setMass(10);
	*/

	/* L */
	massPoint.push_back(new MassPoint(this, Vector2f(0, 0)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, 0)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, -200)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, -400)));
	massPoint.push_back(new MassPoint(this, Vector2f(400, -400)));
	massPoint.push_back(new MassPoint(this, Vector2f(400, -600)));
	massPoint.push_back(new MassPoint(this, Vector2f(200, -600)));
	massPoint.push_back(new MassPoint(this, Vector2f(0, -600)));
	massPoint.push_back(new MassPoint(this, Vector2f(0, -400)));
	massPoint.push_back(new MassPoint(this, Vector2f(0, -200)));

	linkPoints(massPoint[0], massPoint[1]);
	linkPoints(massPoint[0], massPoint[2]);
	linkPoints(massPoint[0], massPoint[9]);
	linkPoints(massPoint[2], massPoint[8]);
	linkPoints(massPoint[2], massPoint[9]);
	linkPoints(massPoint[2], massPoint[1]);
	linkPoints(massPoint[9], massPoint[1]);
	linkPoints(massPoint[9], massPoint[8]);
	linkPoints(massPoint[9], massPoint[3]);
	linkPoints(massPoint[2], massPoint[3]);
	linkPoints(massPoint[8], massPoint[3]);
	linkPoints(massPoint[8], massPoint[7]);
	linkPoints(massPoint[8], massPoint[6]);
	linkPoints(massPoint[6], massPoint[7]);
	linkPoints(massPoint[6], massPoint[3]);
	linkPoints(massPoint[7], massPoint[3]);
	linkPoints(massPoint[6], massPoint[5]);
	linkPoints(massPoint[3], massPoint[5]);
	linkPoints(massPoint[6], massPoint[4]);
	linkPoints(massPoint[3], massPoint[4]);
	linkPoints(massPoint[5], massPoint[4]);

	//massPoint[5]->lock(massPoint[5]->getPosition());

	//*/

	/* O
	massPoint.push_back(new MassPoint(this, Vector2f(0, 0)));
	massPoint.push_back(new MassPoint(this, Vector2f(165.69, 0)));
	massPoint.push_back(new MassPoint(this, Vector2f(282.85, 165.69)));
	massPoint.push_back(new MassPoint(this, Vector2f(282.85, 282.85)));
	massPoint.push_back(new MassPoint(this, Vector2f(165.69, 400)));
	massPoint.push_back(new MassPoint(this, Vector2f(0, 400)));
	massPoint.push_back(new MassPoint(this, Vector2f(-117.16, 282.85)));
	massPoint.push_back(new MassPoint(this, Vector2f(-117.16, 165.69)));

	linkPoints(massPoint[0], massPoint[1]);
	linkPoints(massPoint[1], massPoint[2]);
	linkPoints(massPoint[2], massPoint[3]);
	linkPoints(massPoint[3], massPoint[4]);
	linkPoints(massPoint[4], massPoint[5]);
	linkPoints(massPoint[5], massPoint[6]);
	linkPoints(massPoint[6], massPoint[7]);
	linkPoints(massPoint[7], massPoint[0]);

	linkPoints(massPoint[0], massPoint[4]);
	linkPoints(massPoint[1], massPoint[5]);
	linkPoints(massPoint[2], massPoint[6]);
	linkPoints(massPoint[3], massPoint[7]);
	*/
}


void SoftBody::setBodyCollection(BodyCollection* collection)
{
	this->collection = collection;
}

BodyCollection* SoftBody::getBodyCollection()
{
	return this->collection;
}

void SoftBody::print(RenderWindow& window)
{
	// Print Springs
	for (int i = 0; i < spring.size(); ++i) {
		spring[i]->print(window);

	}

	// Print MassPoints
	for (int i = 0; i < massPoint.size(); ++i) massPoint[i]->print(window);
}

void SoftBody::update(float time)
{
	/*
	if (collection == nullptr) {
		std::cout << "Not updating because SoftBody doesn't have collection." << std::endl;
		return;
	}*/

	// Update Springs
	for (int i = 0; i < spring.size(); ++i) spring[i]->update();

	// Update MassPoints
	for (int i = 0; i < massPoint.size(); ++i) {
		//massPoint[i]->update(time, i == 5);
		massPoint[i]->update(time, false);
		if (massPoint[i]->getPosition().x < left) left = massPoint[i]->getPosition().x;
		if (massPoint[i]->getPosition().x > right) right = massPoint[i]->getPosition().x;
		if (massPoint[i]->getPosition().y < top) top = massPoint[i]->getPosition().y;
		if (massPoint[i]->getPosition().y > bottom) bottom = massPoint[i]->getPosition().y;
	}

	// Update other variables

}

MassPoint* SoftBody::getMassPoint(Vector2f position, float distance)
{
	for (int i = 0; i < massPoint.size(); ++i)
	{
		Vector2f pointPosition = massPoint[i]->getPosition();
		if (pointPosition.x > position.x - distance and pointPosition.x < position.x + distance
			and pointPosition.y > position.y - distance and pointPosition.y < position.y + distance
			and operation::distanceBetween(pointPosition, position) <= distance)
		{
			massPoint[i]->highlight();
			return massPoint[i];
		}
	}
	return nullptr;
}

bool SoftBody::linkPoints(MassPoint* A, MassPoint* B)
{
	std::cout << "Linking (" << A->getPosition().x << ", " << A->getPosition().y << ") and (" << B->getPosition().x << ", " << B->getPosition().y << ")... ";
	// Check if the connection already exists:
	for (int i = 0; i < spring.size(); ++i) {
		if (spring[i]->getConnection(A) == B) {
			std::cout << "That Spring already existed!" << std::endl;
			return false;
		}
	}
	std::cout << std::endl;

	// Create connection:
	Spring* newSpring = new Spring(A, B);
	A->addSpring(newSpring);
	B->addSpring(newSpring);

	// Add connection to body's array:
	this->spring.push_back(newSpring);
}
