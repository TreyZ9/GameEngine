#pragma once

#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsBox
{
private:
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btRigidBody* body;

public:
	PhysicsBox();
	PhysicsBox(btVector3 size, btVector3 position, btScalar mass);
	~PhysicsBox();

	btVector3 getPosition();

	btCollisionShape* getShape();
	btRigidBody* getBody();
};
