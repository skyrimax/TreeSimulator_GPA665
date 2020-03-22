#pragma once

#include "Branch.h"
#include "Vector.h"

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


private:
	Branch* trunk;
	Vector<Branch*> brokenBranches;
};

