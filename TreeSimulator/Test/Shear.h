#pragma once

#include "Matrix.h"

class Shear :
	public Matrix
{
public:
	Shear(double shx, double shy);

	double getShx() const;
	double getShy() const;
};

