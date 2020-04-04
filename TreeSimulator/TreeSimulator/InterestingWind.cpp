#include "InterestingWind.h"

#include <cmath>

InterestingWind::InterestingWind()
	: Wind()
{
}

InterestingWind::~InterestingWind()
{
}

Vector InterestingWind::operator()(double t)
{
	return Vector(0.5*sin(2.5*t)-0.5*sin(1.5*t));
}
