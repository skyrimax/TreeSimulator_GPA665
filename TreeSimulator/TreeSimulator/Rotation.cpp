#include "Rotation.h"

Rotation::Rotation(Axis axis, double angle)
	: TransformationMatrix(), m_angle(angle), m_axis(axis)
{
	switch (axis)
	{
	case Rotation::Axis::X:
		(*this)(0, 0) = 1;
		(*this)(1, 1) = cos(angle);
		(*this)(1, 2) = -sin(angle);
		(*this)(2, 1) = sin(angle);
		(*this)(2, 2) = cos(angle);
		(*this)(3, 3) = 1;
		break;
	case Rotation::Axis::Y:
		(*this)(1, 1) = 1;
		(*this)(0, 0) = cos(angle);
		(*this)(2, 0) = -sin(angle);
		(*this)(0, 2) = sin(angle);
		(*this)(2, 2) = cos(angle);
		(*this)(3, 3) = 1;
		break;
	case Rotation::Axis::Z:
		(*this)(2, 2) = 1;
		(*this)(0, 0) = cos(angle);
		(*this)(0, 1) = -sin(angle);
		(*this)(1, 0) = sin(angle);
		(*this)(1, 1) = cos(angle);
		(*this)(3, 3) = 1;
		break;
	default:
		(*this)(0, 0) = 1;
		(*this)(1, 1) = 1;
		(*this)(2, 2) = 1;
		(*this)(3, 3) = 1;
		break;
	}
}

double Rotation::getAngle() const
{
	return m_angle;
}

Rotation::Axis Rotation::getAxis() const
{
	return m_axis;
}
