#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
	this->overlappingPairCache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver;
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	this->dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

PhysicsManager::~PhysicsManager()
{
	for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		this->dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete this->dynamicsWorld;
	delete this->solver;
	delete this->overlappingPairCache;
	delete this->dispatcher;
	delete this->collisionConfiguration;

	this->collisionShapes.clear();
}

void PhysicsManager::addCollisionShape(btCollisionShape* collisionShape, btRigidBody* body)
{
	this->collisionShapes.push_back(collisionShape);
	this->dynamicsWorld->addRigidBody(body);
}

void PhysicsManager::stepSimulation(const btScalar timeStep, const int maxSubSteps)
{
	this->dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}

btDiscreteDynamicsWorld* PhysicsManager::getDyanmicsWorld()
{
	return this->dynamicsWorld;
}