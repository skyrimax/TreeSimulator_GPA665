#pragma once

#include <string>

#include "array.h"
#include "Vector.h"
#include "Wind.h"
#include "TransformationMatrix.h"

class Branch
{
public:
	Branch();
	Branch(Branch* parent, double length, double dia, double angle,
		double youngModulus, double ultimateTensileStrenght,
		double shearModulus, double ultimateSheerStrenght,
		double density);
	Branch(const Branch& branch);
	Branch(Branch&& branch);
	~Branch();

	Branch& operator=(const Branch& branch);
	Branch& operator=(Branch&& branch);

	void addBranches(array<Branch*> branches);

	double simulate(const Vector& wind, array<Branch*>& brokenBranches);
	std::string printFrame(const TransformationMatrix& matrix, double scale);

	double getLength() const;
	double getDia() const;
	double getAngle() const;
	double getMass() const;
	const Vector& getVelocity() const;
	const Vector& getCenterOfMass() const;
	
	const Vector getExperiencedForce(const Wind& wind) const;

	const Vector getAbsEndCoordinates();
	double getAbsEndOrientation();

	const double getBranchesMass() const;
	const Vector getBranchesVelocity() const;
	const Vector getBranchesCenterOfMass() const;
	const Vector getBranchesExperiencedForce(const Wind& wind) const;

private:
	struct Segment
	{
		Segment(double length=0, double angle=0) :m_length(length), m_angle(angle) {}
		double m_length;
		double m_angle;
	};

	Branch* m_parent;
	array<Branch*> m_children;

	array<Segment> m_segments;

	double m_length;
	double m_dia;
	double m_angle;

	double m_mass;
	double m_density;
	Vector m_velocity;
	Vector m_centerOfMass;

	double m_youngModulus;
	double m_ultimateTensileStrenght;
	double m_shearModulus;
	double m_ultimateSheerStrenght;
};