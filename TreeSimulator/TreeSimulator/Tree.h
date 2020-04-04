#pragma once

#include <string>
#include <random>

#include "Branch.h"
#include "array.h"
#include "Vector.h"
#include "Wind.h"
#include "TransformationMatrix.h"

class Tree
{
public:
	Tree();
	Tree(int maxNbLvls, double minlengthRatio, double maxLengthRatio,
		double minLengthBranch, double minLengthTrunk, double maxLengthTrunk,
		double minDiaRatio, double maxDiaRatio,
		double minDiaBranch, double minDiaTrunk, double maxDiaTrunk,
		int minNbBranches, int maxNbBranches,
		double minOpeningAngle, double maxOpeningAngle,
		double youngModulus, double ultimateTensileStrenght,
		double shearModulus, double ultimateSheerStrenght,
		double density, unsigned long long  seed);
	Tree(const Tree& tree);
	Tree(Tree&& tree);
	~Tree();

	Tree& operator=(const Tree& tree);
	Tree& operator=(Tree&& tree);

	std::string simulate(const Wind& wind);
	void simulateTick(const Vector& wind);
	std::string printFrame(const TransformationMatrix& treeBase, double scale);

private:
	template<class RNG>
	array<Branch*> createBranchLevel(Branch* parent,
		std::uniform_real_distribution<double>& lengthRatioRandDist,
		std::uniform_real_distribution<double>& diaRatioRandDist,
		std::uniform_real_distribution<double>& openingAngleRandDist,
		std::uniform_int_distribution<int> nbBranchesRandDist,
		RNG _Eng, int level, int maxNbLvls,
		double minLengthBranch, double minDiaBranch,
		double youngModulus, double ultimateTensileStrenght,
		double shearModulus, double ultimateSheerStrenght,
		double density);

	Branch* m_trunk;
	array<Branch*> m_brokenBranches;
};

template<class RNG>
inline array<Branch*> Tree::createBranchLevel(Branch * parent,
	std::uniform_real_distribution<double>& lengthRatioRandDist,
	std::uniform_real_distribution<double>& diaRatioRandDist,
	std::uniform_real_distribution<double>& openingAngleRandDist,
	std::uniform_int_distribution<int> nbBranchesRandDist,
	RNG _Eng, int level, int maxNbLvls,
	double minLengthBranch, double minDiaBranch,
	double youngModulus, double ultimateTensileStrenght,
	double shearModulus, double ultimateSheerStrenght,
	double density)
{
	if (level<=maxNbLvls && parent->getLength()>minLengthBranch) {
		// Temporary holder for the branch being created
		Branch* temp;

		// Select a nomber of branches for this level
		int nbBranches = nbBranchesRandDist(_Eng);
		
		// Total opening angle of this level
		double openingAngle = openingAngleRandDist(_Eng);

		// Create the array that will hold the branches of this level,
		// Preallocated the necesary space since the number of branches is known
		array<Branch*> branches(nbBranches);

		// As many times as there are branches on this level...
		for (int i = 0; i < nbBranches; ++i) {
			// Length of the next branch, must be at least the minimum length
			double length = parent->getLength()*lengthRatioRandDist(_Eng);
			if (length < minLengthBranch) {
				length = minLengthBranch;
			}

			// Diameter of the next branch, must be at least the minimum diameter
			double dia = parent->getDia()*diaRatioRandDist(_Eng);
			if (dia < minDiaBranch) {
				dia = minDiaBranch;
			}
			temp= new Branch(parent, length, dia, -openingAngle / 2 + i * openingAngle / (nbBranches - 1),
				youngModulus, ultimateTensileStrenght, shearModulus, ultimateSheerStrenght, density);

			temp->addBranches(createBranchLevel(temp, lengthRatioRandDist, diaRatioRandDist, openingAngleRandDist,
				nbBranchesRandDist, _Eng, level + 1, maxNbLvls, minLengthBranch, minDiaBranch,
				youngModulus, ultimateTensileStrenght, shearModulus, ultimateSheerStrenght, density));

			branches[i] = temp;
		}

		return branches;
	}
	else {
		return array<Branch*>();
	}
}
