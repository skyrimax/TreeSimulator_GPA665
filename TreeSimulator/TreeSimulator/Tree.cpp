#include "Tree.h"

#include <sstream>
#include <utility>

Tree::Tree()
{
	m_trunk = new Branch;
}

Tree::Tree(int maxNbLvls, double minlengthRatio, double maxLengthRatio,
	double minLengthBranch, double minLengthTrunk, double maxLengthTrunk,
	double minDiaRatio, double maxDiaRatio,
	double minDiaBranch, double minDiaTrunk, double maxDiaTrunk,
	int minNbBranches, int maxNbBranches,
	double minOpeningAngle, double maxOpeningAngle,
	double youngModulus, double ultimateTensileStrenght,
	double shearModulus, double ultimateSheerStrenght,
	double density, unsigned long long  seed)
{
	// Creating random number distributions for the numerous randomized characteristics of the branches
	std::uniform_real_distribution<double> lengthRatioRandDist(minlengthRatio, maxLengthRatio);
	std::uniform_real_distribution<double> lengthTrunkRandDist(minLengthTrunk, maxLengthTrunk);
	std::uniform_real_distribution<double> diaRatioRandDist(minDiaRatio, maxDiaRatio);
	std::uniform_real_distribution<double> diaTrunkRandDist(minDiaTrunk, maxDiaTrunk);
	std::uniform_real_distribution<double> openingAngleRandDist(minOpeningAngle, maxOpeningAngle);
	std::uniform_int_distribution<int> nbBranchesRandDist(minNbBranches, maxNbBranches);

	// Creating random number engine with the seed passed as parameter
	std::mt19937_64 gen(seed);

	// Creating the trunk as base for the other branches
	m_trunk = new Branch(nullptr, lengthTrunkRandDist(gen), diaTrunkRandDist(gen), 0,
		youngModulus, ultimateTensileStrenght, shearModulus, ultimateSheerStrenght, density);

	m_trunk->addBranches(createBranchLevel(m_trunk, lengthRatioRandDist, diaRatioRandDist, openingAngleRandDist,
		nbBranchesRandDist, gen, 2, maxNbLvls, minLengthBranch, minDiaBranch,
		youngModulus, ultimateTensileStrenght, shearModulus, ultimateSheerStrenght, density));
}

Tree::Tree(const Tree & tree)
{
	// Using copy assignment opertators do move the data
	// from the copied object over to this object
	m_trunk = tree.m_trunk;
	m_brokenBranches = tree.m_brokenBranches;
}

Tree::Tree(Tree && tree)
{
	// Using move assignment opertators do move the data
	// from the copied object over to this object
	m_trunk = std::move(tree.m_trunk);
	m_brokenBranches = std::move(tree.m_brokenBranches);
}

Tree::~Tree()
{
	delete m_trunk;

	for (size_t i = 0; i < m_brokenBranches.size(); ++i) {
		delete m_brokenBranches[i];
	}
}

Tree & Tree::operator=(const Tree & tree)
{
	// Delete currently allocated memory for this object
	delete m_trunk;

	for (size_t i = 0; i < m_brokenBranches.size(); ++i) {
		delete m_brokenBranches[i];
	}

	// Assign a copy of the truck using copy constructor
	m_trunk = new Branch(*tree.m_trunk);
	// Assign copies of the brocken branches using copy assignment operators
	m_brokenBranches = tree.m_brokenBranches;

	return *this;
}

Tree & Tree::operator=(Tree && tree)
{
	// Delete currently allocated memory for this object
	delete m_trunk;

	for (size_t i = 0; i < m_brokenBranches.size(); ++i) {
		delete m_brokenBranches[i];
	}

	// Move ownership of the passed tree's trunk over to this tree
	m_trunk = tree.m_trunk;
	tree.m_trunk = nullptr;
	// Assign the brocken branches using move assignment operators
	m_brokenBranches = std::move(tree.m_brokenBranches);

	return *this;
}

std::string Tree::printFrame(const TransformationMatrix & treeBase, double scale)
{
	std::stringstream frame;

	frame << m_trunk->printFrame(treeBase, scale);

	for (size_t i = 0; i < m_brokenBranches.size(); ++i) {
		frame << m_brokenBranches[i]->printFrame(treeBase, scale);
	}

	return frame.str();
}
