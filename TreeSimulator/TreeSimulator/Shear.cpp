#include "Shear.h"

Shear::Shear(double shx, double shy)
	: Matrix(3, 3, {1, shx, 0, shy, 1, 0, 0, 0, 1})
{
}

double Shear::getShx() const
{
	return m_matrix[1];
}

double Shear::getShy() const
{
	return m_matrix[3];
}
