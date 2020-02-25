#pragma once
#include "MathUtils.h"

class SignedDistanceField
{
	virtual double dist(Vector position) = 0;
};


class Sphere : public SignedDistanceField
{
	Vector position;
	double radius;
	virtual double dist(Vector position);
};

