#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "SoftBody.h"
#include "FixedBody.h"

class SoftBody;
class FixedBody;
class MassPoint;

class BodyCollection
{
public:
	BodyCollection();
	void add(SoftBody& body);
	void add(FixedBody& body);

	std::vector<FixedBody>* getFixedBodies();

	MassPoint* getMassPoint(Vector2f position, float distance);

	void print(RenderWindow& window);
	void update(float time);

private:
	std::vector<SoftBody> softbody;
	std::vector <FixedBody> fixedbody;
};

