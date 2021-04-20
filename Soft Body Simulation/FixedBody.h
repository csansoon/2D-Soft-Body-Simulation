#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class FixedBody
{
public:
	FixedBody();

	void print(RenderWindow& window);

	void addPoint(Vector2f point);
	void addPoint(float x, float y);

	float getTopMargin();
	float getBottomMargin();
	float getLeftMargin();
	float getRightMargin();

	std::vector<Vector2f> getPoints();

private:
	std::vector<Vector2f> point;

	float top, bottom, left, right;
};

