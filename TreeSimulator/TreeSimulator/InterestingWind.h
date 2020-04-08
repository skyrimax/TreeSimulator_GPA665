#pragma once

#include "Wind.h"

class InterestingWind :
	public Wind
{
public:
	InterestingWind();
	~InterestingWind();

	virtual Vector operator()(double t) const override;
};

