#pragma once

#include "TransformationMatrix.h"

class Rotation :
	public TransformationMatrix
{
public:
	enum class Axis { X, Y, Z };

	Rotation(Axis axis, double angle);

	double getAngle() const;
	Axis getAxis() const;

private:
	double m_angle;
	Axis m_axis;
};

