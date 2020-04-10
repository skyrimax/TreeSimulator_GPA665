#pragma once

#include "array.h"

class Matrix
{
public:
	//Type to indicate size
	typedef unsigned int size_type;

	// Constructors and destructors
	Matrix(size_type nbRows = 1, size_type nbCols = 1);
	Matrix(size_type nbRows, size_type nbCols, std::initializer_list<double> values);
	Matrix(const Matrix& m);
	Matrix(Matrix&& m);
	~Matrix();

	// Copy and move assignment operators
	Matrix& operator=(const Matrix& m);
	Matrix& operator=(Matrix&& m);

	// Element accessing operator
	const double& operator()(size_type row, size_type col) const;
	double& operator()(size_type row, size_type col);

	// Element access method
	const double& at(size_type row, size_type col) const;
	double& at(size_type row, size_type col);

	// Unary operators
	Matrix operator+() const;
	Matrix operator-() const;

	// Arithmetics operators
	Matrix operator+(const Matrix& b) const;
	Matrix operator-(const Matrix& b) const;
	Matrix operator*(const int a) const;
	Matrix operator*(const double a) const;
	Matrix operator*(const Matrix& b) const;
	Matrix operator/(const int b) const;
	Matrix operator/(const double b) const;

	// Inverse operator
	Matrix operator~() const;

	// Caracteristics accessors
	size_type nbRows() const;
	size_type nbCols() const;

	Matrix getSubMatrix(size_type rowToRemove, size_type colToRemove);
	double det();
	Matrix cofactors();
	Matrix transpose();
	Matrix inverse();
	
protected:
	array<double> m_matrix;

	size_type m_nbRows;
	size_type m_nbCols;
};

//Matrix operator+(const Matrix& a);
//Matrix operator-(const Matrix& a);

//Matrix operator+(const Matrix& a, const Matrix& b);
//Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator*(int a, const Matrix& b);
Matrix operator*(double a, const Matrix& b);
//Matrix operator*(const Matrix& a, const Matrix& b);

//Matrix operator~(const Matrix& a);