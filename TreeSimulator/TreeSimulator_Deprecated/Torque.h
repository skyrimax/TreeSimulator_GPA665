#pragma once

#include "Vector.h"

class Torque
{
public:
	Torque();
	Torque(const Vector& force, const Vector& lever);
	Torque(const Torque& t);
	Torque(Torque&& t);
	~Torque();

	Torque& operator=(const Torque& t);
	Torque& operator=(Torque&& t);

	const Vector& getForce() const;
	Vector& getForce();
	const Vector& getLever() const;
	Vector& getLever();
	Vector getTorque() const;

	void changeRef(const Vector& displacement);

private:
	Vector m_force;
	Vector m_lever;
};