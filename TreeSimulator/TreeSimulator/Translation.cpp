#include "Translation.h"

Translation::Translation(double tx, double ty, double tz)
	: TransformationMatrix({1, 0, 0, tx, 0, 1, 0, ty, 0, 0, 1, tz, 0, 0, 0, 1})
{
}

double Translation::getTx() const
{
	return m_matrix[3];
}

double Translation::getTy() const
{
	return m_matrix[7];
}

double Translation::getTz() const
{
	return m_matrix[11];
}
