#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "MassPoint.h"
#include "Spring.h"
#include "BodyCollection.h"
using namespace sf;

class Spring;
class MassPoint;
class BodyCollection;

class SoftBody 
{
public:
	SoftBody();

	void setBodyCollection(BodyCollection* collection);
	BodyCollection* getBodyCollection();

	void print(RenderWindow& window);
	void update(float time);

	void setTotalMass(float mass);
	MassPoint* getMassPoint(Vector2f position, float distance);

private:
	BodyCollection* collection;
	std::vector<MassPoint*> massPoint;
	std::vector<Spring*> spring;

	bool linkPoints(MassPoint* A, MassPoint* B);

	float top, bottom, left, right;
};

