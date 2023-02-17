#pragma once

#include <bullet3/btBulletDynamicsCommon.h>
#include <iostream>

class PhysicsManager
{
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

public:
	PhysicsManager();
	~PhysicsManager();

	void addCollisionShape(btCollisionShape* collisionShape, btRigidBody* body);
	void stepSimulation(const btScalar timeStep, const int maxSubSteps = 10);

	btDiscreteDynamicsWorld* getDyanmicsWorld();
};

