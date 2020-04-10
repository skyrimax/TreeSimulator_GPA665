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

Vector::Vector(const Vector & v)
	: Matrix(v)
{
}

Vector::Vector(Vector && v)
	: Matrix(std::move(v))
{
}

Vector::~Vector()
{
}

Vector & Vector::operator=(const Vector & v)
{
	m_matrix = v.m_matrix;
	m_nbRows = v.m_nbRows;
	m_nbCols = v.m_nbCols;

	return *this;
}

Vector & Vector::operator=(Vector && v)
{
	m_matrix = std::move(v.m_matrix);
	m_nbRows = v.m_nbRows;
	m_nbCols = v.m_nbCols;

	return *this;
}

Vector Vector::operator+(const Vector& b) const
{
	return Vector(m_matrix[0] + b.m_matrix[0], m_matrix[1] + b.m_matrix[1], m_matrix[2] + b.m_matrix[2]);
}

Vector Vector::operator-(const Vector & b) const
{
	return Vector(m_matrix[0] - b.m_matrix[0], m_matrix[1] - b.m_matrix[1], m_matrix[2] - b.m_matrix[2]);
}

Vector & Vector::operator+=(const Vector & b)
{
	m_matrix[0] += b.m_matrix[0];
	m_matrix[1] += b.m_matrix[1];
	m_matrix[2] += b.m_matrix[2];

	return *this;
}

Vector Vector::operator*(const int a) const
{
	return Vector(a*m_matrix[0], a*m_matrix[1], a*m_matrix[2]);
}

Vector Vector::operator*(const double a) const
{
	return Vector(a*m_matrix[0], a*m_matrix[1], a*m_matrix[2]);
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

Vector Vector::unit() const
{
	double norm = this->norm();
	if (norm != 0) {
		return Vector(m_matrix[0] / norm, m_matrix[1] / norm, m_matrix[2] / norm);
	}
	else {
		return Vector();
	}
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
	return static_cast<Vector>(b.operator*(a));
}
