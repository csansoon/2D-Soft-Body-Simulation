#include "FixedBody.h"

FixedBody::FixedBody()
{
	top = 0;
	bottom = 0;
	left = 0;
	right = 0;
}

void FixedBody::print(RenderWindow& window)
{
	if (point.size() > 1)
	{
		int size = point.size();
		VertexArray polygon(LineStrip, size + 1);

		for (int i = 0; i < point.size(); ++i) {
			polygon[i].position = point[i];
		}
		polygon[point.size()].position = point[0];

		window.draw(polygon);
	}
}

void FixedBody::addPoint(Vector2f point)
{
	this->point.push_back(point);

	if (this->point.size() > 1)
	{
		if (point.x < left) left = point.x;
		if (point.x > right) right = point.x;
		if (point.y < top) top = point.y;
		if (point.y > bottom) bottom = point.y;
	}
	else
	{
		left = point.x;
		right = point.x;
		top = point.y;
		bottom = point.y;
	}
}

void FixedBody::addPoint(float x, float y)
{
	this->addPoint(Vector2f(x, y));
}

float FixedBody::getTopMargin()
{
	return top;
}
float FixedBody::getBottomMargin()
{
	return bottom;
}
float FixedBody::getLeftMargin()
{
	return left;
}
float FixedBody::getRightMargin()
{
	return right;
}

std::vector<Vector2f> FixedBody::getPoints()
{
	return point;
}
