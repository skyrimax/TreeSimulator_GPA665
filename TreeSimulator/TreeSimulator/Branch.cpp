#include "Branch.h"

#include <sstream>
#include <math.h>
#include <utility>

#include "Rotation.h"
#include "Translation.h"

Branch::Branch()
	: m_parent(nullptr), m_length(0), m_dia(0), m_angle(0), m_mass(0), m_density(0),
	m_youngModulus(0), m_ultimateTensileStrenght(0), m_shearModulus(0), m_ultimateSheerStrenght(0),
	m_segments(5)
{
}

Branch::Branch(Branch * parent, double length, double dia, double angle,
	double youngModulus, double ultimateTensileStrenght,
	double shearModulus, double ultimateSheerStrenght,
	double density)
	: m_parent(parent), m_length(length), m_dia(dia), m_angle(angle),
	m_mass(length*M_PI*dia*dia*density), m_density(density), m_centerOfMass(0, length/2),
	m_youngModulus(youngModulus), m_ultimateTensileStrenght(ultimateTensileStrenght),
	m_shearModulus(shearModulus), m_ultimateSheerStrenght(ultimateSheerStrenght),
	m_segments(5, Segment(length/5.0))
{
}

Branch::Branch(const Branch & branch)
	: m_parent(branch.m_parent), m_segments(branch.m_segments),
	m_length(branch.m_length), m_dia(branch.m_dia), m_angle(branch.m_angle), m_mass(branch.m_mass),
	m_density(branch.m_density), m_velocity(branch.m_velocity), m_centerOfMass(branch.m_centerOfMass),
	m_youngModulus(branch.m_youngModulus), m_ultimateTensileStrenght(branch.m_ultimateTensileStrenght),
	m_shearModulus(branch.m_shearModulus), m_ultimateSheerStrenght(branch.m_ultimateSheerStrenght)
{
	// Will hold a copy of the child branch
	Branch* copy(nullptr);

	for (size_t i = 0; i < branch.m_children.size(); ++i) {
		// Creates a copy of the child branch
		copy = new Branch(*branch.m_children[i]);
		// Add the copy to the child branches of this object
		m_children.push_back(copy);
	}
}

Branch::Branch(Branch && branch)
	: m_parent(branch.m_parent), m_children(std::move(branch.m_children)), m_segments(std::move(branch.m_segments)),
	m_length(branch.m_length), m_dia(branch.m_dia), m_angle(branch.m_angle), m_mass(branch.m_mass),
	m_density(branch.m_density), m_velocity(std::move(branch.m_velocity)), m_centerOfMass(std::move(branch.m_centerOfMass)),
	m_youngModulus(branch.m_youngModulus), m_ultimateTensileStrenght(branch.m_ultimateTensileStrenght),
	m_shearModulus(branch.m_shearModulus), m_ultimateSheerStrenght(branch.m_ultimateSheerStrenght)
{
}

Branch::~Branch()
{
	// Deletes all child branches
	for (size_t i = 0; i < m_children.size(); ++i) {
		delete m_children[i];
	}
}

Branch & Branch::operator=(const Branch & branch)
{
	// Uses the copy assignment operators of the various classes to
	// assign the values of the copied object over to this object
	m_parent = branch.m_parent;
	m_segments = branch.m_segments;
	m_length = branch.m_length;
	m_dia = branch.m_dia;
	m_density = branch.m_density;
	m_velocity = branch.m_velocity;
	m_centerOfMass = branch.m_centerOfMass;
	m_youngModulus = branch.m_youngModulus;
	m_ultimateTensileStrenght = branch.m_ultimateTensileStrenght;
	m_shearModulus = branch.m_shearModulus;
	m_ultimateSheerStrenght = branch.m_ultimateSheerStrenght;

	// Copy of the child branches
	// Will hold a copy of the child branch
	Branch* copy(nullptr);

	for (size_t i = 0; i < branch.m_children.size(); ++i) {
		// Creates a copy of the child branch
		copy = new Branch(*branch.m_children[i]);
		// Add the copy to the child branches of this object
		m_children.push_back(copy);
	}

	return *this;
}

Branch & Branch::operator=(Branch && branch)
{
	// Uses the move assignment operators of the complex classes 
	// and the copy assignment operators of the basic classes to
	// assign the values of the copied object over to this object
	m_parent = branch.m_parent;
	m_children = std::move(branch.m_children);
	m_segments = std::move(branch.m_segments);
	m_length = branch.m_length;
	m_dia = branch.m_dia;
	m_density = branch.m_density;
	m_velocity = std::move(branch.m_velocity);
	m_centerOfMass = std::move(branch.m_centerOfMass);
	m_youngModulus = branch.m_youngModulus;
	m_ultimateTensileStrenght = branch.m_ultimateTensileStrenght;
	m_shearModulus = branch.m_shearModulus;
	m_ultimateSheerStrenght = branch.m_ultimateSheerStrenght;

	return *this;
}

void Branch::addBranches(array<Branch*> branches)
{
	for (size_t i = 0; i < branches.size(); ++i) {
		m_children.push_back(branches[i]);
	}
}

std::string Branch::printFrame(const TransformationMatrix & h_trunk_parent, double scale)
{
	std::stringstream frame;
	TransformationMatrix h_trunk_tip(h_trunk_parent * Rotation(Rotation::Axis::Z, m_angle));

	Vector base;

	for (size_t i = 0; i < m_segments.size(); ++i) {
		h_trunk_tip = h_trunk_tip * Rotation(Rotation::Axis::Z, m_segments[i].m_angle);

		frame << "dlin " << h_trunk_tip * base << " " << h_trunk_tip * Vector(0, m_segments[i].m_length*scale) << '\n';

		h_trunk_tip = h_trunk_tip * Translation(0, m_segments[i].m_length*scale);
	}

	for (size_t i = 0; i < m_children.size(); ++i) {
		frame << m_children[i]->printFrame(h_trunk_tip, scale);
	}

	return frame.str();
}

double Branch::getLength() const
{
	return m_length;
}

double Branch::getDia() const
{
	return m_dia;
}

double Branch::getAngle() const
{
	return m_angle;
}

double Branch::getMass() const
{
	return m_mass;
}

const Vector & Branch::getVelocity() const
{
	return m_velocity;
}

const Vector & Branch::getCenterOfMass() const
{
	return m_centerOfMass;
}
