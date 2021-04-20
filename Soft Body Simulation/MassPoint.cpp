#include "MassPoint.h"
#include "operation.h"

MassPoint::MassPoint(SoftBody* body, Vector2f pos)
{
	this->body = body;
	this->position = pos;

	this->mass = 1;
	this->force = Vector2f(0, 0);
	this->velocity = Vector2f(0, 0);
}

void MassPoint::setPosition(Vector2f pos)
{
	this->position = pos;
}

void MassPoint::setVelocity(Vector2f vel)
{
	this->velocity = vel;
}

void MassPoint::setMass(float mass)
{
	this->mass = mass;
}

void MassPoint::lock(Vector2f lockPosition)
{
	position = lockPosition;
	locked = true;
}

void MassPoint::unlock()
{
	locked = false;
}

void MassPoint::highlight()
{
	doHighlight = true;
}

Vector2f MassPoint::getPosition()
{
	return this->position;
}

Vector2f MassPoint::getVelocity()
{
	return velocity;
}

void MassPoint::addSpring(Spring* spring)
{
	this->spring.push_back(spring);
}

void MassPoint::addForce(Vector2f force)
{
	this->force += force;
}

void MassPoint::manageCollision(float time, FixedBody& fixedbody, bool log)
{
	if (	position.y > fixedbody.getTopMargin()
		and	position.y < fixedbody.getBottomMargin()
		and position.x > fixedbody.getLeftMargin()
		and position.x < fixedbody.getRightMargin())
	{
		
		// Detect if there is actually a collision
		std::vector<Vector2f> polygonPoint = fixedbody.getPoints();
		if (polygonPoint.size() < 3) return;

		Vector2f last = Vector2f(position.x - velocity.x * time, position.y - velocity.y * time);
		float mv = (position.y - last.y) / (position.x - last.x);
		float nv = last.y - mv * last.x;

		int count = 0;
		Vector2f exitPoint;
		Vector2f exitVector;
		bool exitFound = false;

		for (int i = 0; i < polygonPoint.size(); ++i) {

			Vector2f A, B;
			A = polygonPoint[i];
			if (i < polygonPoint.size() - 1) B = polygonPoint[i + 1];
			else B = polygonPoint[0];

			if (A == position or B == position or A == B) return;

			if (operation::segment_intersection(A, B, position, Vector2f(fixedbody.getLeftMargin(), position.y)).intersects) {
				count++;
			}

			operation::intersection intersection = operation::segment_intersection(A, B, position, last);
			if (intersection.intersects)
			{	
				operation::location exitLocation = operation::closestPoint(position, A, B);

				

				Vector2f localExitPoint;
				if (exitLocation.before) localExitPoint = A;
				else if (exitLocation.after) localExitPoint = B;
				else localExitPoint = Vector2f(exitLocation.x, exitLocation.y);

				if (exitFound == false or operation::distanceBetween(Vector2f(localExitPoint.x, localExitPoint.y), last) < operation::distanceBetween(exitPoint, last))
				{
					exitPoint = Vector2f(localExitPoint.x, localExitPoint.y);
					exitVector = Vector2f(exitLocation.x - position.x, exitLocation.y - position.y);

					exitFound = true;

					if (log) {
						std::cout << "Colisión detectada con el vértice [" << A.x << ", " << A.y << "]-[" << B.x << ", " << B.y << "]." << std::endl;
						std::cout << "Posición de salida encontrada en [" << exitPoint.x << ", " << exitPoint.y << "].  " << operation::direction(exitVector) << std::endl;
					}
				}
				
			}

		}

		if (log and !exitFound and count % 2 == 1)
		{
			std::cout << "[ERROR]: Masspoint inside but exit not found." << std::endl;
			std::cout << std::endl;
		}

		if (exitFound and count % 2 == 1)
		{
			if (exitPoint.x > 1800) {
				std::cout << std::endl;
			}
			
			if (exitPoint == position) {
				velocity = -velocity;
				return;
			}

			//Vector2f exitVector = exitPoint - position;
			exitVector = operation::normalize(exitVector);

			//if (log) std::cout << "Found an exit from [" << position.x << ", " << position.y << "] towards (" << exitVector.x << ", " << exitVector.y << ")." << std::endl;

			position = exitPoint;

			float dot = operation::dotProduct(exitVector, velocity);
			Vector2f oldVelocity = velocity;
			velocity = Vector2f(velocity.x - (2 * dot * exitVector.x), velocity.y - (2 * dot * exitVector.y));
			velocity = Vector2f(velocity.x * 0.9, velocity.y * 0.9);

			if (log) std::cout << "Velocidad actualizada: (" << velocity.x << ", " << velocity.y << ").  " << operation::direction(velocity) << std::endl;
			if (log) std::cout << "Posición actualizada: [" << position.x << ", " << position.y << "]." << std::endl;

			if (log and position.x >= 1800)
			{
				std::cout << std::endl;
			}
		}

	}
	return;
}

void MassPoint::update(float time, bool log)
{

	if (locked) return;

	if (log)
	{
		std::cout << std::endl << "################################" << std::endl;
		std::cout << "Posición actual = [" << position.x << ", " << position.y << "]." << std::endl;
		std::cout << "Velocidad actual = (" << velocity.x << ", " << velocity.y << ").  " << operation::direction(velocity) << std::endl;
	}

	if (body == nullptr) {
		std::cout << "Not updating because masspoint doesn't have body." << std::endl;
		return;
	}
	

	/** Reset Forces */
	force = Vector2f(0, 0);


	/** Add Forces */
	
	// Gravity
	Vector2f gravity = Vector2f(0, 980 * mass);
	force += gravity;

	// Spring forces
	for (int i = 0; i < spring.size(); ++i) {
		force += spring[i]->getForce(this);
	}

	/** Euler Integration */
	velocity += Vector2f((force.x * time) / mass, (force.y * time) / mass);
	position += Vector2f(velocity.x * time, velocity.y * time);

	if (log)
	{
		std::cout << "Velocidad nueva = (" << velocity.x << ", " << velocity.y << ").  " << operation::direction(velocity) << std::endl;
		std::cout << "Posición nueva = [" << position.x << ", " << position.y << "]." << std::endl;
	}

	/** Check Collisions */
	std::vector<FixedBody>* fixedbodies = body->getBodyCollection()->getFixedBodies();
	for (int i = 0; i < fixedbodies->size(); ++i)
	{
		manageCollision(time, (*fixedbodies)[i], log);
	}


	if (log) {
		std::cout << "################################" << std::endl;
	}
}

void MassPoint::print(RenderWindow& window)
{
	CircleShape circle;
	circle.setRadius(20.0f);
	circle.setFillColor(Color::Red);
	circle.setOrigin(Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setPosition(position);

	if (locked) {
		circle.setOutlineThickness(10.0f);
		circle.setOutlineColor(Color::White);
	}

	if (doHighlight)
	{
		Color circleColor = circle.getFillColor();
		circleColor += Color::Color(128, 128, 128, 0); 
		//circleColor.r += 20;
		//circleColor.g += 20;
		//circleColor.b += 20;
		circle.setFillColor(circleColor);

		doHighlight = false;
	}

	window.draw(circle);
}
