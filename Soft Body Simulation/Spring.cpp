#include "Spring.h"
#include "operation.h"

Spring::Spring(MassPoint* A, MassPoint* B)
{
	this->A = A;
	this->B = B;
	this->originalLength = operation::distanceBetween(A->getPosition(), B->getPosition());

	this->stiffness = 400;
	this->damping = 10;
}

MassPoint* Spring::getConnection(MassPoint* masspoint)
{
	if (masspoint == A) return B;
	else if (masspoint == B) return A;
	else return nullptr;
}

Vector2f Spring::getForce(MassPoint* masspoint)
{
	if (masspoint == A) return forceA;
	else return forceB;
}

void Spring::update()
{
	if (A == nullptr or B == nullptr) {
		std::cout << "Not updating because string doesn't have masspoint." << std::endl;
		return;
	}

	Vector2f posA = A->getPosition();
	Vector2f posB = B->getPosition();

	currentLength = operation::distanceBetween(posA, posB);
	if (currentLength == 0) return;
	Vector2f AtoB = Vector2f((posB.x - posA.x) / currentLength, (posB.y - posA.y) / currentLength);

	springForce = stiffness * (currentLength - originalLength);

	Vector2f difVel = Vector2f(B->getVelocity().x - A->getVelocity().x, B->getVelocity().y - A->getVelocity().y);
	dampingForce = operation::dotProduct(AtoB, difVel) * damping;

	float totalForce = springForce + dampingForce;

	forceA = Vector2f(AtoB.x * totalForce, AtoB.y * totalForce);
	forceB = Vector2f(-AtoB.x * totalForce, -AtoB.y * totalForce);
}

void Spring::print(RenderWindow& window)
{
	Vertex line[2];
	line[0] = A->getPosition();
	line[1] = B->getPosition();

	Color lineColor = Color(255, 255, 0, 255);

	if (currentLength < originalLength) // Turns red
	{
		lineColor.g = (currentLength * 255.0f) / (originalLength);
	}

	else if (currentLength > originalLength) // Turns white
	{
		lineColor.b = ((currentLength - originalLength) * 255.0f) / (originalLength * 0.5f);
	}

	line[0].color = lineColor;
	line[1].color = lineColor;

	window.draw(line, 2, sf::Lines);
}