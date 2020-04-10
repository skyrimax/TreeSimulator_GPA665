#pragma once

#include "Vector.h"

Vector airResistance(const Vector& wind, double length, double dia, double angularVelocity, double orientation);
Vector angularVelocityToLinearVelocity(double s, double angularVelocity, double orientation);

double dragCoefficient(double angle);
double projectedArea(double angle, double dia, double length);
double torqueFromFlexion(double angle, double youngModulus, double radius, double length);
double massOfCylinder(double density, double radius, double length);
double inertiaOfCylinder(double density, double radius, double length);