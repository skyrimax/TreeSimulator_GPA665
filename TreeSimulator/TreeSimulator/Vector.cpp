#include "Vector.h"
#include <stdexcept>

Vector::Vector(double x, double y, double z)
	: Matrix(4, 1, {x, y, z, 1})
{
}

Vector::Vector(const Matrix& m)
	: Matrix(4, 1)
{
	if (m.nbRows() == 4 && m.nbCols() == 1) {
		m_matrix[0] = m(0, 0);
		m_matrix[1] = m(1, 0);
		m_matrix[2] = m(2, 0);
		m_matrix[3] = m(3, 0);
	}
	else
	{
		throw std::length_error("Matrix::_M_Matrix_cannot_be_converted_to_vector");
	}
}

double Vector::getX() const
{
	return m_matrix[0];
}

double Vector::getY() const
{
	return m_matrix[1];
}

double Vector::getZ() const
{
	return m_matrix[2];
}

double Vector::norm() const
{
	return sqrt(m_matrix[0] * m_matrix[0] + m_matrix[1] * m_matrix[1] + m_matrix[2] * m_matrix[2]);
}

double Vector::orientation() const
{
	return atan2(m_matrix[1], m_matrix[0]);
}

std::ostream& operator<<(std::ostream & output, const Vector & vector)
{
	return output << (int)round(vector.getX()) << " " << (int)round(vector.getY());
}

Vector operator*(const TransformationMatrix& a, const Vector& b)
{
	return static_cast<Vector>(a.operator*(b));
}

Vector operator*(double a, const Vector & b)
{
	return static_cast<Vector>(b.operator*(b));
}
