#include "operation.h"
#include <iostream>
using namespace sf;

operation::intersection operation::segment_intersection(Vector2f A, Vector2f B, Vector2f C, Vector2f D)
{
    operation::intersection response;
	response.x = 0;
	response.y = 0;
	response.intersects = false;

	if (A == C or A == D)
	{
		response.x = A.x;
		response.y = A.y;
		response.intersects = true;

		return response;
	}

	if (B == C or B == D)
	{
		response.x = B.x;
		response.y = B.y;
		response.intersects = true;

		return response;
	}

	if (A.x == B.x) // If line AB is vertical (m = infinite)
	{
		if (C.x == D.x) // Both lines are vertical (parallel)
		{
			return response;
		}

		float CDm = (D.y - C.y) / (D.x - C.x);
		float CDn = C.y - CDm * C.x;
		
		response.x = A.x;
		response.y = CDm * response.x + CDn;
	}

	else if (C.x == D.x) // If line AB is vertical (m = infinite)
	{
		float ABm = (B.y - A.y) / (B.x - A.x);
		float ABn = A.y - ABm * A.x;

		response.x = C.x;
		response.y = ABm * response.x + ABn;
	}

	else
	{
		float ABm = (B.y - A.y) / (B.x - A.x);
		float ABn = A.y - ABm * A.x;

		float CDm = (D.y - C.y) / (D.x - C.x);
		float CDn = C.y - CDm * C.x;

		response.x = (ABn - CDn) / (CDm - ABm);
		response.y = ABm * response.x + ABn;
	}

	
	float minX_AB = fminf(A.x, B.x) - 0.05;
	float maxX_AB = fmaxf(A.x, B.x) + 0.05;
	float minY_AB = fminf(A.y, B.y) - 0.05;
	float maxY_AB = fmaxf(A.y, B.y) + 0.05;

	float minX_CD = fminf(C.x, D.x) - 0.05;
	float maxX_CD = fmaxf(C.x, D.x) + 0.05;
	float minY_CD = fminf(C.y, D.y) - 0.05;
	float maxY_CD = fmaxf(C.y, D.y) + 0.05;
	

	if (	response.x >= minX_AB and response.x <= maxX_AB and response.y >= minY_AB and response.y <= maxY_AB
		and response.x >= minX_CD and response.x <= maxX_CD and response.y >= minY_CD and response.y <= maxY_CD)
	{
		response.intersects = true;
	}

	else
	{
	}

	return response;
}

operation::location operation::closestPoint(Vector2f P, Vector2f A, Vector2f B)
{
	operation::location response;
	response.before = false;
	response.after = false;

	Vector2f AB = B - A;
	AB = normalize(AB);

	Vector2f v = P - A;
	float d = dotProduct(v, AB);

	response.x = A.x + AB.x * d;
	response.y = A.y + AB.y * d;

	if (d < 0) response.before = true;
	else if (d > distanceBetween(A, B)) response.after = true;

	return response;
}

float operation::distanceBetween(Vector2f A, Vector2f B)
{
	return sqrtf(powf(A.x - B.x, 2) + powf(A.y - B.y, 2));
}

Vector2f operation::normalize(Vector2f vector)
{
	float modulo = distanceBetween(Vector2f(0, 0), vector);
	return Vector2f(vector.x / modulo, vector.y / modulo);
}

float operation::dotProduct(Vector2f A, Vector2f B)
{
	return (A.x * B.x) + (A.y * B.y);
}


std::string operation::direction(Vector2f vector)
{
	float angle = atan2f(vector.x, vector.y);
	float pi = 3.141592;

	float direction = angle / (pi / 8);


	std::string right = ">";
	std::string left = "<";
	std::string down = "V";
	std::string up = "^";
	std::string up_right = "^>";
	std::string up_left = "<^";
	std::string down_right = "V>";
	std::string down_left = "<V";


	if (direction < 1) return down;	// DOWN
	if (direction < 3) return down_right;	// DOWN RIGHT
	if (direction < 5) return right;	// RIGHT
	if (direction < 7) return up_right;		// UP RIGHT
	if (direction < 9) return up;	// UP
	if (direction < 11) return up_left;	// UP LEFT
	if (direction < 13) return left;	// LEFT
	if (direction < 15) return down_left;	// DOWN LEFT
	return down;						// DOWN
	
}