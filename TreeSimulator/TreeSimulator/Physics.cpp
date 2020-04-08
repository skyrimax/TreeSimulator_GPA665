#include "Physics.h"

Vector airResistance(const Vector& wind, double length, double dia, double angularVelocity, double orientation)
{
	Vector windRelativeToObj(wind - angularVelocityToLinearVelocity(length / 2, angularVelocity, orientation));
	double orientationWind = windRelativeToObj.orientation();

	return ((1.2*pow(windRelativeToObj.norm(), 2)*projectedArea(orientationWind, dia, length)*dragCoefficient(orientationWind)) / 2)*windRelativeToObj.unit();
}

Vector angularVelocityToLinearVelocity(double s, double angularVelocity, double orientation)
{
	return Vector(-s*angularVelocity*sin(orientation), s*angularVelocity*cos(orientation));
}

double dragCoefficient(double angle)
{
	return 0.11*cos(2 * angle) + 0.71;
}

double projectedArea(double angle, double dia, double length)
{
	return (pow(dia, 2)*sin(angle)*M_PI) / 4 + length * dia*cos(angle);
}

double torqueFromFlexion(double angle, double youngModulus, double radius)
{
	return (-32 * youngModulus*pow(radius, 4)*tan(angle / 2)) / (9 * M_PI);
}

double massOfCylinder(double density, double radius, double length)
{
	return density * length*M_PI*pow(radius, 2);
}

double inertiaOfCylinder(double density, double radius, double length)
{
	return massOfCylinder(density, radius, length)*(3 * pow(radius, 2) + pow(length, 2)) / 12;
}
