#include "Matrix.h"

#include <cmath>
#include <stdexcept>
#include <utility>

Matrix::Matrix(size_type nbRows, size_type nbCols)
	: m_matrix(nbRows*nbCols), m_nbRows(nbRows), m_nbCols(nbCols)
{
	for (size_type i = 0; i < m_matrix.size(); ++i) {
		m_matrix[i] = 0;
	}
}

Matrix::Matrix(size_type nbRows, size_type nbCols, std::initializer_list<double> values)
	: m_matrix(nbRows*nbCols), m_nbRows(nbRows), m_nbCols(nbCols)
{
	if (values.size() != nbRows * nbCols) {
		throw std::length_error("Matrix::_M_nb_arguments_and_size_dont_match");
	}

	size_type i = 0;

	for (auto const& a : values) {
		m_matrix[i] = a;

		++i;
	}
}

Matrix::Matrix(const Matrix & m)
	: m_matrix(m.m_matrix), m_nbRows(m.m_nbRows), m_nbCols(m.m_nbCols)
{
}

Matrix::Matrix(Matrix && m)
	: m_matrix(std::move(m.m_matrix)), m_nbRows(m.m_nbRows), m_nbCols(m.m_nbCols)
{
}

Matrix::~Matrix()
{
}

Matrix & Matrix::operator=(const Matrix & m)
{
	m_matrix = m.m_matrix;
	m_nbRows = m.m_nbRows;
	m_nbCols = m.m_nbCols;

	return *this;
}

Matrix & Matrix::operator=(Matrix && m)
{
	m_matrix = std::move(m.m_matrix);
	m_nbRows = m.m_nbRows;
	m_nbCols = m.m_nbCols;

	return *this;
}

const double & Matrix::operator()(size_type row, size_type col) const
{
	return m_matrix[row*m_nbCols + col];
}

double & Matrix::operator()(size_type row, size_type col)
{
	return m_matrix[row*m_nbCols + col];
}

const double & Matrix::at(size_type row, size_type col) const
{
	if (row < m_nbRows && col < m_nbCols) {
		return m_matrix[row*m_nbCols + col];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

double & Matrix::at(size_type row, size_type col)
{
	if (row < m_nbRows && col < m_nbCols) {
		return m_matrix[row*m_nbCols + col];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

Matrix Matrix::operator+() const
{
	return Matrix(*this);
}

Matrix Matrix::operator-() const
{
	return -1 * (*this);
}

Matrix Matrix::operator+(const Matrix & b) const
{
	if (m_nbRows != b.m_nbRows || m_nbCols != b.m_nbCols) {
		throw std::length_error("Matrix::_M_nb_arguments_and_size_dont_match");
	}
	else {
		Matrix c(m_nbRows, m_nbCols);

		for (size_type i = 0; i < m_matrix.size(); ++i) {
			c.m_matrix[i] = m_matrix[i] + b.m_matrix[i];
		}

		return c;
	}
}

Matrix Matrix::operator-(const Matrix & b) const
{
	if (m_nbRows != b.m_nbRows || m_nbCols != b.m_nbCols) {
		throw std::length_error("Matrix::_M_nb_arguments_and_size_dont_match");
	}
	else {
		Matrix c(m_nbRows, m_nbCols);

		for (size_type i = 0; i < m_matrix.size(); ++i) {
			c.m_matrix[i] = m_matrix[i] - b.m_matrix[i];
		}

		return c;
	}
}

Matrix Matrix::operator*(const int a) const
{
	Matrix c(m_nbRows, m_nbCols);

	for (size_type i = 0; i < m_matrix.size(); ++i) {
		c.m_matrix[i] = a * m_matrix[i];
	}

	return c;
}

Matrix Matrix::operator*(const double a) const
{
	Matrix c(m_nbRows, m_nbCols);

	for (size_type i = 0; i < m_matrix.size(); ++i) {
		c.m_matrix[i] = a * m_matrix[i];
	}

	return c;
}

Matrix Matrix::operator*(const Matrix & b) const
{
	if (m_nbCols != b.m_nbRows) {
		throw std::length_error("Matrix::_M_size_of_matrices_not_valid_for_multiplication");
	}
	else {
		Matrix c(m_nbRows, b.m_nbCols);

		for (size_type i = 0; i < c.m_nbRows; ++i) {
			for (size_type j = 0; j < c.m_nbCols; ++j) {
				for (size_type k = 0; k < m_nbCols; ++k) {
					c(i, j) += (*this)(i, k)*b(k, j);
				}
			}
		}

		return c;
	}
}

Matrix Matrix::operator/(const int b) const
{
	Matrix c(m_nbRows, m_nbCols);

	for (size_type i = 0; i < m_matrix.size(); ++i) {
		c.m_matrix[i] = m_matrix[i] / b;
	}

	return c;
}

Matrix Matrix::operator/(const double b) const
{
	Matrix c(m_nbRows, m_nbCols);

	for (size_type i = 0; i < m_matrix.size(); ++i) {
		c.m_matrix[i] = m_matrix[i] / b;
	}

	return c;
}

Matrix Matrix::operator~() const
{
	return Matrix(*this).inverse();
}

Matrix::size_type Matrix::nbRows() const
{
	return m_nbRows;
}

Matrix::size_type Matrix::nbCols() const
{
	return m_nbCols;
}

Matrix Matrix::getSubMatrix(size_type rowToRemove, size_type colToRemove)
{
	Matrix subMatrix(m_nbRows - 1, m_nbCols - 1);
	int x = 0;
	int y = 0;

	for (size_type i = 0; i < m_nbRows; ++i) {
		y = 0;

		if (i != rowToRemove) {
			for (size_type j = 0; j < m_nbCols; ++j) {
				if (j != colToRemove) {
					subMatrix(x, y) = (*this)(i, j);

					++y;
				}
			}

			++x;
		}
	}

	return subMatrix;
}

double Matrix::det()
{
	if (m_nbRows != m_nbCols) {
		throw std::length_error("Matrix::_M_matrix_must_be_square");
	}
	else if (m_nbRows == 1 && m_nbCols == 1) {
		return m_matrix[0];
	}
	else if (m_nbRows == 2 && m_nbCols == 2) {
		return m_matrix[0] * m_matrix[3] - m_matrix[1] * m_matrix[2];
	}
	else {
		double det = 0;

		for (size_type i = 0; i < m_nbCols; ++i) {
			det += pow(-1, i) * m_matrix[i] * this->getSubMatrix(0, i).det();
		}

		return det;
	}
}

Matrix Matrix::cofactors()
{
	Matrix cof(m_nbRows, m_nbCols);

	for (size_type i = 0; i < m_nbRows; ++i) {
		for (size_type j = 0; j < m_nbCols; ++j) {
			cof(i, j) = pow(-1, i + j) * (this->getSubMatrix(i, j).det());
		}
	}

	return cof;
}

Matrix Matrix::transpose()
{
	Matrix trans(m_nbCols, m_nbRows);

	for (size_type i = 0; i < m_nbCols; ++i) {
		for (size_type j = 0; j < m_nbRows; ++j) {
			trans(i, j) = (*this)(j, i);
		}
	}

	return trans;
}

Matrix Matrix::inverse()
{
	if (m_nbRows != m_nbCols) {
		throw std::length_error("Matrix::_M_matrix_must_be_square");
	}
	else {
		return (this->cofactors().transpose()) / (this->det());
	}
}

Matrix operator*(int a, const Matrix & b)
{
	return Matrix(b).operator*(a);
}

Matrix operator*(double a, const Matrix & b)
{
	return Matrix(b).operator*(a);
}

Matrix operator~(const Matrix & a)
{
	return Matrix();
}
