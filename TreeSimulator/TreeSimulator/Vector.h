#pragma once

#include <iostream>

#include "Matrix.h"
#include "TransformationMatrix.h"

class Vector :
	public Matrix
{
public:
	Vector(double x = 0, double y = 0, double z = 0);
	Vector(const Matrix& m);

	friend std::ostream& operator<<(std::ostream& output, const Vector& vector);

	double getX() const;
	double getY() const;
	double getZ() const;
	double norm() const;
	double orientation() const;
};

Vector operator*(const TransformationMatrix& a, const Vector& b);
Vector operator*(double a, const Vector& b);