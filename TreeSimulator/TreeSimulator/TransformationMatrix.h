#pragma once

#include "Matrix.h"

class TransformationMatrix :
	public Matrix
{
public:
	TransformationMatrix();
	TransformationMatrix(std::initializer_list <double> values);
	TransformationMatrix(const Matrix& m);
	~TransformationMatrix();
};

TransformationMatrix operator*(const TransformationMatrix& a, const TransformationMatrix& b);