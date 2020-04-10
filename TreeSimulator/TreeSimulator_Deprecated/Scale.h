#pragma once

#include "TransformationMatrix.h"

class Scale :
	public TransformationMatrix
{
public:
	Scale(double sx, double sy, double sz);

	double getSx() const;
	double getSy() const;
	double getSz() const;
};

