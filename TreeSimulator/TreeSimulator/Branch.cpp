#include "Branch.h"

#include <sstream>
#include <math.h>
#include <utility>

#include "Rotation.h"
#include "Translation.h"
#include "Physics.h"

Branch::Branch()
	: m_parent(nullptr), m_length(0), m_dia(0), m_angle(0), m_mass(0), m_density(0),
	m_youngModulus(0), m_ultimateTensileStrenght(0), m_shearModulus(0), m_ultimateSheerStrenght(0),
	m_segments(5), m_isTrunk(false)
{
}

Branch::Branch(Branch * parent, double length, double dia, double angle,
	double youngModulus, double ultimateTensileStrenght,
	double shearModulus, double ultimateSheerStrenght,
	double density, bool isTrunk)
	: m_parent(parent), m_length(length), m_dia(dia), m_angle(angle),
	m_mass(length*M_PI*dia*dia*density/4), m_density(density),
	m_youngModulus(youngModulus), m_ultimateTensileStrenght(ultimateTensileStrenght),
	m_shearModulus(shearModulus), m_ultimateSheerStrenght(ultimateSheerStrenght),
	m_segments(5, Segment(length/5.0)), m_isTrunk(isTrunk)
{
}

Branch::Branch(const Branch & branch)
	: m_parent(branch.m_parent), m_segments(branch.m_segments),
	m_length(branch.m_length), m_dia(branch.m_dia), m_angle(branch.m_angle), m_mass(branch.m_mass),
	m_density(branch.m_density), m_isTrunk(branch.m_isTrunk),
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
	m_density(branch.m_density), m_isTrunk(branch.m_isTrunk),
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
	m_youngModulus = branch.m_youngModulus;
	m_ultimateTensileStrenght = branch.m_ultimateTensileStrenght;
	m_shearModulus = branch.m_shearModulus;
	m_ultimateSheerStrenght = branch.m_ultimateSheerStrenght;
	m_isTrunk = branch.m_isTrunk;

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
	m_youngModulus = branch.m_youngModulus;
	m_ultimateTensileStrenght = branch.m_ultimateTensileStrenght;
	m_shearModulus = branch.m_shearModulus;
	m_ultimateSheerStrenght = branch.m_ultimateSheerStrenght;
	m_isTrunk = branch.m_isTrunk;

	return *this;
}

void Branch::addBranches(array<Branch*> branches)
{
	for (size_t i = 0; i < branches.size(); ++i) {
		m_children.push_back(branches[i]);
	}
}

void Branch::simulate(const Vector & wind, double duration, array<Branch*>& brokenBranches)
{
	array<Torque> forces(getChildrenExperiencedForces(wind));
	array<double> angularAcceleration;
	Vector centerOfMass(getChildrenCenterOfMass());
	Vector displacement;
	Vector lever;
	Vector aeroDynamicDrag;
	Vector weight;
	Torque aeroDynamicDragTorque;
	Torque weightTorque;
	double orientation;
	double inertia(getChildrenInertia());
	double lengthSegment;
	double angle;
	double masseSegment;
	double massBranches(getChildrenMass());
	double torque;

	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	for (size_t i = 0; i < m_segments.size(); ++i) {
		orientation += m_segments[i].m_angle;
	}

	forces.reserve(forces.size() + 2 * m_segments.size());

	for (int i = (int)m_segments.size() - 1; i >= 0; --i) {
		lengthSegment = m_segments[i].m_length;
		angle = m_segments[i].m_angle;
		displacement = Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));
		torque = 0;
		masseSegment = massOfCylinder(m_density, m_dia / 2, lengthSegment);

		for (size_t j = 0; j < forces.size(); ++j) {
			forces[j].changeRef(displacement);

			torque += forces[j].getTorque().getZ();
		}

		aeroDynamicDrag = airResistance(wind, lengthSegment, m_dia, m_segments[i].m_angularVelocity, orientation);
		weight = Vector(0, -9.807*masseSegment);
		lever = Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation));

		aeroDynamicDragTorque = Torque(aeroDynamicDrag, lever);
		weightTorque = Torque(weight, lever);

		torque += aeroDynamicDragTorque.getTorque().getZ() + weightTorque.getTorque().getZ();
		torque += torqueFromFlexion(angle, m_youngModulus, m_dia / 2);

		centerOfMass += displacement;

		inertia += massBranches * pow(centerOfMass.norm(), 2) + 
			inertiaOfCylinder(m_density, m_dia / 2, lengthSegment) + masseSegment * pow(lengthSegment / 2, 2);

		angularAcceleration.push_back(torque / inertia);
		

		centerOfMass = Vector((massBranches*centerOfMass.getX() + masseSegment * lengthSegment / 2 * cos(orientation)) / (massBranches + masseSegment),
			(massBranches*centerOfMass.getY() + masseSegment * lengthSegment / 2 * sin(orientation)) / (massBranches + masseSegment),
			centerOfMass.getZ());

		massBranches += masseSegment;

		forces.push_back(aeroDynamicDragTorque);
		forces.push_back(weightTorque);
		orientation -= m_segments[i].m_angle;
	}

	for (size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->simulate(wind, duration, brokenBranches);
	}

	if (m_segments.size() == angularAcceleration.size()) {
		for (size_t i = 0; i < m_segments.size(); ++i) {
			m_segments[i].m_angularVelocity += angularAcceleration[i] * duration;
			m_segments[i].m_angle += m_segments[i].m_angularVelocity * duration;
		}
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
	return m_mass + getChildrenMass();
}

double Branch::getChildrenMass() const
{
	double mass(0);

	for (size_t i = 0; i < m_children.size(); ++i) {
		mass += m_children[i]->getMass();
	}

	return mass;
}

//const Vector & Branch::getVelocity() const
//{
//	return Vector();
//}

array<Torque> Branch::getExperiencedForces(const Vector & wind) const
{
	array<Torque> forces(getChildrenExperiencedForces(wind));
	array<Torque> forcesOnSegments;
	double orientation;
	double length;
	Vector displacement;
	Vector lever;
	Vector aeroDynamicDrag;
	Vector weight;

	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	for (size_t i = 0; i < m_segments.size(); ++i) {
		orientation += m_segments[i].m_angle;
		length = m_segments[i].m_length;
		aeroDynamicDrag = airResistance(wind, m_length, m_dia, m_segments[i].m_angularVelocity, orientation);
		weight = Vector(0, -9.807*massOfCylinder(m_density, m_dia/2, length));
		lever = Vector(length / 2 * cos(orientation), length / 2 * sin(orientation))+displacement;

		forcesOnSegments.push_back(Torque(aeroDynamicDrag, lever));
		forcesOnSegments.push_back(Torque(weight, lever));

		displacement += Vector(length*cos(orientation), length*sin(orientation));
	}

	for (size_t i = 0; i < forces.size(); ++i) {
		forces[i].changeRef(displacement);
	}

	forces.reserve(forces.size() + forcesOnSegments.size());

	for (size_t i = 0; i < forcesOnSegments.size(); ++i) {
		forces.push_back(std::move(forcesOnSegments[i]));
	}

	return forces;
}

array<Torque> Branch::getChildrenExperiencedForces(const Vector & wind) const
{
	array<Torque> forces;
	array<Torque> temp;

	for (size_t i = 0; i < m_children.size(); ++i) {
		temp = m_children[i]->getExperiencedForces(wind);

		forces.reserve(forces.size() + temp.size());

		for (size_t j = 0; j < temp.size(); ++j) {
			forces.push_back(temp[j]);
		}
	}

	return forces;
}

double Branch::getAbsEndOrientation() const
{
	double deviation(0);

	for (size_t i = 0; i < m_segments.size(); ++i) {
		deviation += m_segments[i].m_angle;
	}

	if (m_isTrunk) {
		return M_PI_2 + deviation;
	}
	else {
		return m_parent->getAbsEndOrientation() + m_angle + deviation;
	}
}

double Branch::getInertia() const
{
	array<double> inertia;
	array<Vector> centersOfMass;
	array<double> masses;
	Vector displacement;
	double orientation;
	double lengthSegment;
	double totalInertia(0);

	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	for (size_t i = 0; i < m_segments.size(); ++i) {
		orientation += m_segments[i].m_angle;
		lengthSegment = m_segments[i].m_length;

		centersOfMass.push_back(Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation)) + displacement);
		inertia.push_back(inertiaOfCylinder(m_density, m_dia / 2, lengthSegment));
		masses.push_back(massOfCylinder(m_density, m_dia / 2, lengthSegment));

		displacement += Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));
	}

	centersOfMass.push_back(getChildrenCenterOfMass() + displacement);
	inertia.push_back(getChildrenInertia());
	masses.push_back(getChildrenMass());

	if (centersOfMass.size() == inertia.size() && centersOfMass.size() == masses.size()) {
		for (size_t i = 0; i < centersOfMass.size(); ++i) {
			totalInertia += inertia[i] + masses[i] * pow(centersOfMass[i].norm(), 2);
		}
	}

	return totalInertia;
}

double Branch::getChildrenInertia() const
{
	double totalInertia(0);

	for (size_t i = 0; i < m_children.size(); ++i) {
		totalInertia += m_children[i]->getInertia();
	}

	return totalInertia;
}

Vector Branch::getCenterOfMass() const
{
	array<double>masses;
	array<Vector>centersOfMass;
	Vector displacement;
	double orientation;
	double lengthSegment;
	double totalMass(0);
	double numX(0);
	double numY(0);
	double numZ(0);

	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	for (size_t i = 0; i < m_segments.size(); ++i) {
		orientation += m_segments[i].m_angle;
		lengthSegment = m_segments[i].m_length;

		centersOfMass.push_back(Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation)) + displacement);
		masses.push_back(massOfCylinder(m_density, m_dia / 2, lengthSegment));

		displacement += Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));
	}

	centersOfMass.push_back(getChildrenCenterOfMass() + displacement);
	masses.push_back(getChildrenMass());

	if (centersOfMass.size() == masses.size()) {
		for (size_t i = 0; i < centersOfMass.size(); ++i) {
			numX += masses[i] * centersOfMass[i].getX();
			numY += masses[i] * centersOfMass[i].getY();
			numZ += masses[i] * centersOfMass[i].getZ();
			totalMass += masses[i];
		}
	}

	if (totalMass != 0) {
		return Vector(numX / totalMass, numY / totalMass, numZ / totalMass);
	}
	else {
		return Vector();
	}
}

Vector Branch::getChildrenCenterOfMass() const
{
	array<double>masses;
	array<Vector>centersOfMass;
	double totalMass(0);
	double numX(0);
	double numY(0);
	double numZ(0);

	for (size_t i = 0; i < m_children.size(); ++i) {
		centersOfMass.push_back(m_children[i]->getCenterOfMass());
		masses.push_back(m_children[i]->getMass());
	}

	if (masses.size() == centersOfMass.size()) {
		for (size_t i = 0; i < centersOfMass.size(); ++i) {
			numX += masses[i] * centersOfMass[i].getX();
			numY += masses[i] * centersOfMass[i].getY();
			numZ += masses[i] * centersOfMass[i].getZ();
			totalMass += masses[i];
		}
	}

	if (totalMass != 0) {
		return Vector(numX / totalMass, numY / totalMass, numZ / totalMass);
	}
	else {
		return Vector();
	}
}
