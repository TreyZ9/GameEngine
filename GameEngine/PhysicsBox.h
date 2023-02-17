#pragma once

#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsBox
{
private:
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btRigidBody* body;

public:
	PhysicsBox() = default;
	PhysicsBox(const btVector3& size, const btVector3& position, const btScalar mass);
	~PhysicsBox() = default;

	btVector3 getPosition();

	btCollisionShape* getShape();
	btRigidBody* getBody();
};
