#include "Scale.h"

Scale::Scale(double sx, double sy, double sz)
	: TransformationMatrix({sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1})
{
}

double Scale::getSx() const
{
	return m_matrix[0];
}

double Scale::getSy() const
{
	return m_matrix[5];
}

double Scale::getSz() const
{
	return m_matrix[10];
}
