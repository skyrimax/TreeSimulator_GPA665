#pragma once

#include <string>

#include "array.h"
#include "Vector.h"
#include "Torque.h"
#include "Wind.h"
#include "TransformationMatrix.h"

class Branch
{
public:
	Branch();
	Branch(Branch* parent, double length, double dia, double angle,
		double youngModulus, double ultimateTensileStrenght,
		double shearModulus, double ultimateSheerStrenght,
		double density, bool isTrunk);
	Branch(const Branch& branch);
	Branch(Branch&& branch);
	~Branch();

	Branch& operator=(const Branch& branch);
	Branch& operator=(Branch&& branch);

	void addBranches(array<Branch*> branches);

	virtual void simulate(const Vector& wind, double duration, array<Branch*>& brokenBranches);
	std::string printFrame(const TransformationMatrix& matrix, double scale);

	double getLength() const;
	double getDia() const;
	double getAngle() const;
	//const Vector& getVelocity() const;
	
	array<Torque> getExperiencedForces(const Vector& wind) const;
	array<Torque> getChildrenExperiencedForces(const Vector& wind) const;

	double getAbsEndOrientation() const;

	//const Vector getBranchesVelocity() const;
	double getMass() const;
	double getChildrenMass() const;
	double getInertia() const;
	double getChildrenInertia() const;
	Vector getCenterOfMass() const;
	Vector getChildrenCenterOfMass() const;

private:
	struct Segment
	{
		Segment(double length = 0, double angle = 0, double angularVelocity = 0) :m_length(length), m_angle(angle), m_angularVelocity(angularVelocity) {}
		double m_length;
		double m_angle;
		double m_angularVelocity;
	};

	Branch* m_parent;
	array<Branch*> m_children;

	array<Segment> m_segments;

	double m_length;
	double m_dia;
	double m_angle;

	double m_mass;
	double m_density;

	double m_youngModulus;
	double m_ultimateTensileStrenght;
	double m_shearModulus;
	double m_ultimateSheerStrenght;

	bool m_isTrunk;
};