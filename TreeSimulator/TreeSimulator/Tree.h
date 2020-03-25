#pragma once

#include <string>
#include <iostream>

#include "Branch.h"
#include "array.h"
#include "Vector.h"
#include "Wind.h"

class Tree
{
public:
	Tree();
	Tree(int maxNbLvls, double minRatio, double maxRatio,
		double minLengthBranch, double minLengthTrunk, double maxLengthTrunk,
		int minNbBranches, int maxNbBranches,
		double minAngleOuverture, double maxAngleOuverture);
	Tree(const Tree& tree);
	Tree(Tree&& tree);
	~Tree();

	Tree& operator=(const Tree& tree);
	Tree& operator=(Tree&& tree);

	std::string simulate(Wind wind);
	void simulateTick(Vector wind);
	std::string printFrame();

	friend std::ostream& operator<<(std::ostream &output, const Tree& tree);

private:
	Branch* m_trunk;
	array<Branch*> m_brokenBranches;
};

