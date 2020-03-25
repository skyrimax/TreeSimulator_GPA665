#pragma once

#include <string>
#include <iostream>

#include "array.h"
#include "Vector.h"

class Branch
{
public:
	Branch();
	Branch(Branch* parent, double lenght, double angle);
	Branch(const Branch& branch);
	Branch(Branch&& branch);
	~Branch();

	Branch& operator=(const Branch& branch);
	Branch& operator=(Branch&& branch);

	double simulate(Vector wind, array<Branch*> brokenBranches);
	std::string printFrame();

	friend std::ostream operator<<(std::ostream& output, const Branch& branch);

	const double& getWeight() const;
	const Vector& getVelocity() const;
	const Vector& getCenterOfMass() const;
	
	const Vector& getExperiencedForce(const Wind& wind) const;
};

