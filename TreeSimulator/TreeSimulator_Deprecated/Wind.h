#pragma once

#include <string>

#include "Vector.h"
#include "Translation.h"

class Wind
{
public:
	Wind();
	~Wind();

	virtual Vector operator()(double t)const = 0;

	std::string printWindProfile(const TransformationMatrix& originOfProfile, unsigned int width, double tTotal) const;
	std::string printCursor(const TransformationMatrix& originOfProfile, unsigned int width, double t, double tTotal) const;

	double getMaxX(double begin, double end) const;
	double getMinX(double begin, double end) const;
	double getMaxY(double begin, double end) const;
	double getMinY(double begin, double end) const;
};
