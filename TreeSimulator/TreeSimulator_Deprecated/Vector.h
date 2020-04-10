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
	Vector(const Vector& v);
	Vector(Vector&& v);
	~Vector();

	Vector& operator=(const Vector& v);
	Vector& operator=(Vector&& v);

	friend std::ostream& operator<<(std::ostream& output, const Vector& vector);
	Vector operator+(const Vector& b) const;
	Vector operator-(const Vector& b) const;
	Vector& operator+=(const Vector& b);
	Vector operator*(const int a) const;
	Vector operator*(const double a) const;

	double getX() const;
	double getY() const;
	double getZ() const;
	double norm() const;
	double orientation() const;
	Vector unit() const;
};

Vector operator*(const TransformationMatrix& a, const Vector& b);
Vector operator*(double a, const Vector& b);