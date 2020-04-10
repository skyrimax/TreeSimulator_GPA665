#pragma once

#include "TransformationMatrix.h"

class Translation :
	public TransformationMatrix
{
public:
	Translation(double tx = 0, double ty = 0, double tz = 0);

	double getTx() const;
	double getTy() const;
	double getTz() const;
};
