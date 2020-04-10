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
	// Forces of the system
	array<Torque> forces(getChildrenExperiencedForces(wind));
	Vector aeroDynamicDrag;
	Vector weight;
	Torque aeroDynamicDragTorque;
	Torque weightTorque;

	// The angular acceleration for each segment
	array<double> angularAcceleration(m_segments.size());

	// Centers of mass
	array<Vector> segmentsCentersOfMass(m_segments.size());
	Vector childrenCenterOfMass(getChildrenCenterOfMass());
	// Center of mass of the system
	Vector systemCenterOfMass;

	// Inertia of the system' bodies around respective center of mass
	array<double> segmentsInertia(m_segments.size());
	double childrenInertia(getChildrenInertia());
	// Inertia of the system
	double systemInertia;

	// Masses of the system
	array<double> segmentsMass(m_segments.size());
	double childrenMass(getChildrenMass());
	// Total mass of the system
	double totalMass;

	// Displacement from the base to the tip of the segment
	Vector displacement;
	// Absolute orientation of the segment
	double orientation;
	// Pretty self-explanatory
	double lengthSegment;
	// Deformation angle of the segment
	double angle;
	// Torque at the base of the segment
	double torque;
	// holders for the calculation of the center of mass
	double numX;
	double numY;
	double numZ;

	// Get absolute orientation of the branch
	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	// Gather statig data about the segments
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// set orientation for this segment
		orientation += m_segments[i].m_angle;

		lengthSegment = m_segments[i].m_length;

		segmentsMass[i] = massOfCylinder(m_density, m_dia / 2, lengthSegment);
		segmentsInertia[i] = inertiaOfCylinder(m_density, m_dia / 2, lengthSegment);
		segmentsCentersOfMass[i] = Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation));
	}

	// Reserve additionnal space for the additionnal forces to save time
	forces.reserve(forces.size() + 2 * m_segments.size());

	// Calculate the experienced forces, torque and resulting angular acceleration
	// for each segment, from last to first
	for (int i = (int)m_segments.size() - 1; i >= 0; --i) {
		// Store data that will need to be accessed multiple time, spares numerous function calls
		lengthSegment = m_segments[i].m_length;
		angle = m_segments[i].m_angle;

		// Set displacement or reference frame from the previous iteration's reference
		// to the base of the current segment
		displacement = Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));

		// Initialise torque to 0 to prepare for the summation that will come
		torque = 0;
		// Initialize system's mass to 0 to prepare for the summation that will come
		totalMass = 0;
		// Initialize summation holders to 0 to prepare for the summation that will come
		numX = 0;
		numY = 0;
		numZ = 0;
		// Initialize system's inertia to 0 to prepare for the summation that will come
		systemInertia = 0;

		// For each forces...
		for (size_t j = 0; j < forces.size(); ++j) {
		// Move lever from last reference point to base of this segment
			forces[j].changeRef(displacement);

			// Add the torque around reference point generated by this force
			torque += forces[j].getTorque().getZ();
		}

		// Move the reference of the center of mass of the segments
		// to this reference frame
		for (size_t j = i + 1; j < m_segments.size(); ++j) {
			segmentsCentersOfMass[j] += displacement;
		}

		// Move the reference of the center of mass of the child branches
		// to this reference frame
		childrenCenterOfMass += displacement;

		// Calculate the forces applied to this segent
		aeroDynamicDrag = airResistance(wind, lengthSegment, m_dia, m_segments[i].m_angularVelocity, orientation);
		weight = Vector(0, -9.807*segmentsMass[i]);

		// Will calculate the torque generated by these forces
		// Air resisante averages out as one net force applied at the center of mass
		aeroDynamicDragTorque = Torque(aeroDynamicDrag, segmentsCentersOfMass[i]);
		// The weight is applied at the center of mass
		weightTorque = Torque(weight, segmentsCentersOfMass[i]);

		// Add torque generated by air resistance and weight to the torque around the base of this segment
		torque += aeroDynamicDragTorque.getTorque().getZ() + weightTorque.getTorque().getZ();
		torque += torqueFromFlexion(angle, m_youngModulus, m_dia / 2, lengthSegment);

		// Calculate the centre of mass of the entire system

		// For every segments from this until the end...
		for (size_t j = i; j < m_segments.size(); ++j) {
			// Σxj*mj
			numX += segmentsCentersOfMass[j].getX()*segmentsMass[j];
			// Σyj*mj
			numY += segmentsCentersOfMass[j].getY()*segmentsMass[j];
			// Σyj*mj
			numZ += segmentsCentersOfMass[j].getZ()*segmentsMass[j];

			// Σmj
			totalMass += segmentsMass[j];
		}

		// Add the mass and center of mass of the child branches to the equation
		// Σxj*mj
		numX += childrenCenterOfMass.getX()*childrenMass;
		// Σxj*mj
		numY += childrenCenterOfMass.getY()*childrenMass;
		// Σxj*mj
		numZ += childrenCenterOfMass.getZ()*childrenMass;

		// Σmj
		totalMass += childrenMass;

		if (totalMass != 0) {
			//							xcm=(Σxj*mj)/Σmj  ycm=(Σyj*mj)/Σmj  zcm=(Σzj*mj)/Σmj
			systemCenterOfMass = Vector(numX / totalMass, numY / totalMass, numZ / totalMass);
		}
		else {
			systemCenterOfMass = Vector();
		}

		// Calculate inertia of the system around it's center of mass' Z axis
		// Icm=ΣIj+m*dj : Parallel axis theorem
		for (size_t j = i; j < m_segments.size(); ++j) {
			// Icm		  +Σ		Ij					mj		  *		(		pos cmj			  -		pos cm system )		   ^2
			systemInertia += segmentsInertia[j] + segmentsMass[j] * pow((segmentsCentersOfMass[j] - systemCenterOfMass).norm(), 2);
		}
		// Add inertia of child branches
		systemInertia += childrenInertia + childrenMass * pow((childrenCenterOfMass - systemCenterOfMass).norm(), 2);
		// Report system' inertia from it's center of mass to the Z axis at the base to this segment
		// using the parallel axis theorem
		systemInertia += totalMass * pow(systemCenterOfMass.norm(), 2);

		// Angular acceleration around the base of this segment
		// α=ΣM/I
		angularAcceleration[i] = (torque / systemInertia);

		// Add the forces applied to this segment to the list of forces so it
		// can be use by the previous segment
		forces.push_back(std::move(aeroDynamicDragTorque));
		forces.push_back(std::move(weightTorque));

		// Get orientation of the previous segment, in preparation for it's iteration
		orientation -= m_segments[i].m_angle;
	}

	// Simulate all child branches
	for (size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->simulate(wind, duration, brokenBranches);
	}

	// Move after the child branches to not influence their own simulations
	if (m_segments.size() == angularAcceleration.size()) {
		for (size_t i = 0; i < m_segments.size(); ++i) {
			// ωf=ωi+α*t;
			m_segments[i].m_angularVelocity += angularAcceleration[i] * duration;
			// θf=θi+ω*t
			m_segments[i].m_angle += m_segments[i].m_angularVelocity * duration;

			// Limit θ to [-π, π] to prevent things going out of control, will also reset speed for good measures
			if (m_segments[i].m_angle < -M_PI) {
				m_segments[i].m_angle = -M_PI;
				m_segments[i].m_angularVelocity = 0;
			}
			else if (m_segments[i].m_angle > M_PI) {
				m_segments[i].m_angle = M_PI;
				m_segments[i].m_angularVelocity = 0;
			}
		}
	}
}

std::string Branch::printFrame(const TransformationMatrix & h_trunk_parent, double scale)
{
	// Stringstream that will this branches' animation code as well as it's children's
	std::stringstream frame;
	// Transfomation matrix transforming each segemnt's coordinates system
	// to the display's coordinates system
	TransformationMatrix h_trunk_tip(h_trunk_parent * Rotation(Rotation::Axis::Z, m_angle));

	// Origin pointer used for the base of each segment
	Vector base;

	// For each segement...
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// Move transformation matrix from tip of previous segment to bas of this one
		// Rotation of m_angle around it's Z axis
		h_trunk_tip = h_trunk_tip * Rotation(Rotation::Axis::Z, m_segments[i].m_angle);

		// Transforms the two vectors representing the base and the tip of this segement to the coordinates system
		// of the display before pushing their X and Y coordinates on the stream to for a line instruction
		// Also adds and "end of line" character fo the next line
		frame << "dlin " << h_trunk_tip * base << " " << h_trunk_tip * Vector(0, m_segments[i].m_length*scale) << '\n';

		// Moves the transformation matrix from the base of this segment to it's tips
		// via a translation along it's length
		h_trunk_tip = h_trunk_tip * Translation(0, m_segments[i].m_length*scale);
	}

	// Add the instruction for all this branche's children to it's stream
	for (size_t i = 0; i < m_children.size(); ++i) {
		frame << m_children[i]->printFrame(h_trunk_tip, scale);
	}

	// Return the string contained in the stream
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
	// The mass of this branche plus the mass of it's children
	return m_mass + getChildrenMass();
}

double Branch::getChildrenMass() const
{
	double mass(0);

	// Add the mass of all the children
	for (size_t i = 0; i < m_children.size(); ++i) {
		mass += m_children[i]->getMass();
	}

	return mass;
}

array<Torque> Branch::getExperiencedForces(const Vector & wind) const
{
	// Allocates array that will contain all the forces on this branch as well
	// as well as all it's children's experienced foreces
	// Is initialized with thechildren experienced forces
	array<Torque> forces(getChildrenExperiencedForces(wind));
	// Array that will hold the forces experience for each segment
	array<Torque> forcesOnSegments(m_segments.size() * 2);
	
	// Absolute orientation of the segment
	double orientation;
	// Length of the segment
	double length;

	// Displacement from the base to the tip of the segment
	Vector displacement;

	// Lever upon witch the force generates a torque,
	// referenced from the base the branch
	Vector lever;

	// Force of the aerodynamic drag on a segment
	Vector aeroDynamicDrag;
	// Force of the weight of a segment
	Vector weight;

	// Get absolute orientation of the branch
	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	// For each segment
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// set orientation for this segment
		orientation += m_segments[i].m_angle;

		// Save lenght of segment since if wil be used several times
		length = m_segments[i].m_length;

		// Calculate the forces applied to this segent
		aeroDynamicDrag = airResistance(wind, m_length, m_dia, m_segments[i].m_angularVelocity, orientation);
		weight = Vector(0, -9.807*massOfCylinder(m_density, m_dia/2, length));

		// The forces are applied at the center of mass of this segment
		// The displacement is used to get the center of mass referenced at the base of th branch
		lever = Vector(length / 2 * cos(orientation), length / 2 * sin(orientation))+displacement;

		// Both forces are put in the array with their respective lever
		forcesOnSegments[2*i]=Torque(aeroDynamicDrag, lever);
		forcesOnSegments[2*i+1]=Torque(weight, lever);

		// Add this segment's displacement to the overall one to prepare the next segment
		displacement += Vector(length*cos(orientation), length*sin(orientation));
	}

	// Move lever from the tip to base of this branch
	for (size_t i = 0; i < forces.size(); ++i) {
		forces[i].changeRef(displacement);
	}

	// Reserve additionnal space in the array to keep addition of forces at O1
	forces.reserve(forces.size() + forcesOnSegments.size());

	// Add the forces on the segments to the forces experienced
	for (size_t i = 0; i < forcesOnSegments.size(); ++i) {
		forces.push_back(std::move(forcesOnSegments[i]));
	}

	// Return the forces experienced
	return forces;
}

array<Torque> Branch::getChildrenExperiencedForces(const Vector & wind) const
{
	// Will hold all the forces on the child branches
	// The reference is the tip of the parent branch
	array<Torque> forces;
	// Will hold the forces on one child branch
	array<Torque> temp;

	// For each child branch...
	for (size_t i = 0; i < m_children.size(); ++i) {
		// Get all the forces on this child brach
		temp = m_children[i]->getExperiencedForces(wind);

		// Reserve additionnal space to keep the addition of forces as O1
		forces.reserve(forces.size() + temp.size());

		// Move all the forces from this child branch
		// over to the overall array
		for (size_t j = 0; j < temp.size(); ++j) {
			forces.push_back(std::move(temp[j]));
		}
	}

	// Return the forces on all child branches
	return forces;
}

double Branch::getAbsEndOrientation() const
{
	// Hold the sum of all deviation angles of the segments
	double deviation(0);

	// For each segment...
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// Add its deviation angle to the total
		deviation += m_segments[i].m_angle;
	}

	if (m_isTrunk) {
		// The trunk is at 90° with the ground,
		// add the segment's deviation to get the orientation of it's tip
		return M_PI_2 + deviation;
	}
	else {
		// Branches are at m_angle base deviation with their parent's orientation,
		// add the segment's deviation to get the orientation of it's tip
		return m_parent->getAbsEndOrientation() + m_angle + deviation;
	}
}

double Branch::getInertia() const
{
	// Inertia of the child branches around their center of mass
	double childrenInertia(getChildrenInertia());
	// Inertia of the system
	double systemInertia(0);

	// Center of mass of one segment
	Vector segmentCentersOfMass;
	// Child branches centers of mass
	Vector childrenCenterOfMass(getChildrenCenterOfMass());
	// Center of mass of the system
	Vector systemCenterOfMass(getCenterOfMass());

	// Masses of the system
	double childrenMass(getChildrenMass());

	// Displacement from the base to the tip of the segment
	Vector displacement;
	// Absolute orientation of the segment
	double orientation;
	// Pretty self-explanatory
	double lengthSegment;

	// Get absolute orientation of the branch
	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	// For each segement...
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// Store data that will need to be accessed multiple time, spares numerous function calls
		lengthSegment = m_segments[i].m_length;
		// set orientation for this segment
		orientation += m_segments[i].m_angle;

		// Calculate center of mass of this segment reference on system's center of mass
		segmentCentersOfMass = Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation))
			+ displacement - systemCenterOfMass;

		// Icm		  +Σ						Ij
		systemInertia += inertiaOfCylinder(m_density, m_dia/2, lengthSegment) +
			// +					mj					   *				d^2
			massOfCylinder(m_density, m_dia/2, lengthSegment)*pow(segmentCentersOfMass.norm(), 2);

		// Set displacement of reference frame to base of the next segment
		displacement += Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));
	}
	// Add inertia of child branches
	systemInertia += childrenInertia + childrenMass * (childrenCenterOfMass - systemCenterOfMass).norm();

	return systemInertia;
}

double Branch::getChildrenInertia() const
{
	// Center of mass of the system
	Vector systemCenterOfMass(getChildrenCenterOfMass());

	// Inertia of the system
	double systemInertia(0);

	for (size_t i = 0; i < m_children.size(); ++i) {
		systemInertia += m_children[i]->getInertia() + m_children[i]->getMass()
			*pow((m_children[i]->getCenterOfMass() - systemCenterOfMass).norm(), 2);
	}

	return systemInertia;
}

Vector Branch::getCenterOfMass() const
{
	// Centers of mass
	Vector segmentCentersOfMass;
	Vector childrenCenterOfMass(getChildrenCenterOfMass());

	// Masses of the system
	double segmentsMass;
	double childrenMass(getChildrenMass());
	// Total mass of the system
	double totalMass(0);

	// Displacement from the base to the tip of the segment
	Vector displacement;
	// Absolute orientation of the segment
	double orientation;
	// Pretty self-explanatory
	double lengthSegment;
	// holders for the calculation of the center of mass
	double numX(0);
	double numY(0);
	double numZ(0);

	// Get absolute orientation of the branch
	if (m_isTrunk) {
		orientation = M_PI_2;
	}
	else {
		orientation = m_parent->getAbsEndOrientation() + m_angle;
	}

	// For each segment
	for (size_t i = 0; i < m_segments.size(); ++i) {
		// Store data that will need to be accessed multiple time, spares numerous function calls
		orientation += m_segments[i].m_angle;
		lengthSegment = m_segments[i].m_length;

		segmentCentersOfMass = Vector(lengthSegment / 2 * cos(orientation), lengthSegment / 2 * sin(orientation)) + displacement;
		segmentsMass = massOfCylinder(m_density, m_dia / 2, lengthSegment);

		// Σxj*mj
		numX += segmentCentersOfMass.getX()*segmentsMass;
		// Σyj*mj
		numY += segmentCentersOfMass.getY()*segmentsMass;
		// Σyj*mj
		numZ += segmentCentersOfMass.getZ()*segmentsMass;

		// Σmj
		totalMass += segmentsMass;

		displacement += Vector(lengthSegment*cos(orientation), lengthSegment*sin(orientation));
	}

	// Add the mass and center of mass of the child branches to the equation
	// Σxj*mj
	numX += childrenCenterOfMass.getX()*childrenMass;
	// Σxj*mj
	numY += childrenCenterOfMass.getY()*childrenMass;
	// Σxj*mj
	numZ += childrenCenterOfMass.getZ()*childrenMass;

	if (totalMass != 0) {
		//			  xcm=(Σxj*mj)/Σmj  ycm=(Σyj*mj)/Σmj  zcm=(Σzj*mj)/Σmj
		return Vector(numX / totalMass, numY / totalMass, numZ / totalMass);
	}
	else {
		return Vector();
	}
}

Vector Branch::getChildrenCenterOfMass() const
{
	// Centers of mass
	Vector childCenterOfMass;

	// Masses of the system
	double childMass;
	// Total mass of the system
	double totalMass(0);

	// holders for the calculation of the center of mass
	double numX(0);
	double numY(0);
	double numZ(0);

	// For each child branch
	for (size_t i = 0; i < m_children.size(); ++i) {
		// Store data that will need to be accessed multiple time, spares numerous long function calls
		childCenterOfMass = m_children[i]->getCenterOfMass();
		childMass = m_children[i]->getMass();

		// Σxj*mj
		numX += childCenterOfMass.getX()*childMass;
		// Σyj*mj
		numY += childCenterOfMass.getY()*childMass;
		// Σyj*mj
		numZ += childCenterOfMass.getZ()*childMass;

		// Σmj
		totalMass += childMass;
	}

	if (totalMass != 0) {
		return Vector(numX / totalMass, numY / totalMass, numZ / totalMass);
	}
	else {
		return Vector();
	}
}
