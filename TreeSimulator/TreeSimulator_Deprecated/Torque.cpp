#include "Torque.h"

#include <utility>

Torque::Torque()
{
}

Torque::Torque(const Vector& force, const Vector& lever)
	: m_force(force), m_lever(lever)
{
}

Torque::Torque(const Torque & t)
	: m_force(t.m_force), m_lever(t.m_lever)
{
}

Torque::Torque(Torque && t)
	: m_force(std::move(t.m_force)), m_lever(std::move(t.m_lever))
{
}

Torque::~Torque()
{
}

Torque & Torque::operator=(const Torque & t)
{
	m_force = t.m_force;
	m_lever = t.m_lever;

	return *this;
}

Torque & Torque::operator=(Torque && t)
{
	m_force = std::move(t.m_force);
	m_lever = std::move(t.m_lever);

	return *this;
}

const Vector & Torque::getForce() const
{
	return m_force;
}

Vector & Torque::getForce()
{
	return m_force;
}

const Vector & Torque::getLever() const
{
	return m_lever;
}

Vector & Torque::getLever()
{
	return m_lever;
}

Vector Torque::getTorque() const
{
	return Vector(m_lever.getY()*m_force.getZ() - m_force.getY()*m_lever.getZ(),
		m_lever.getZ()*m_force.getX() - m_force.getZ()*m_lever.getX(),
		m_lever.getX()*m_force.getY() - m_force.getX()*m_lever.getY());
}

void Torque::changeRef(const Vector & displacement)
{
	m_lever += displacement;
}
