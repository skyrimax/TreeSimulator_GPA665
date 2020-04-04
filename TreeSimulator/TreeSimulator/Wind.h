#pragma once

#include <string>

#include "Vector.h"
#include "Translation.h"

class Wind
{
public:
	Wind();
	~Wind();

	virtual Vector operator()(double t)=0;

	std::string printWindProfile(TransformationMatrix& originOfProfile, double tTotal);
	std::string printCursor(TransformationMatrix& originOfProfile, double t, double tTotal);

	double getMaxX(double begin, double end);
	double getMinX(double begin, double end);
	double getMaxY(double begin, double end);
	double getMinY(double begin, double end);
};
