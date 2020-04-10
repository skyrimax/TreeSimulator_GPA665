#include "TransformationMatrix.h"
#include <stdexcept>

TransformationMatrix::TransformationMatrix()
	: Matrix(4, 4)
{
	for (size_type i = 0; i < 4; ++i) {
		(*this)(i, i) = 1;
	}
}

TransformationMatrix::TransformationMatrix(std::initializer_list <double> values)
	: Matrix(4, 4, values)
{
}

TransformationMatrix::TransformationMatrix(const Matrix & m)
	: Matrix(4, 4)
{
	if (m.nbRows() == 4 && m.nbCols() == 4) {
		for (size_type i = 0; i < m.nbRows(); ++i) {
			for (size_type j = 0; j < m.nbCols(); ++j) {
				(*this)(i, j) = m(i, j);
			}
		}
	}
	else
	{
		throw std::length_error("Matrix::_M_Matrix_cannot_be_converted_to_vector");
	}
}

TransformationMatrix::~TransformationMatrix()
{
}

TransformationMatrix operator*(const TransformationMatrix & a, const TransformationMatrix & b)
{
	return static_cast<TransformationMatrix>(a.operator*(b));
}
