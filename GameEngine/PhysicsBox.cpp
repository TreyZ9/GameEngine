#include "PhysicsBox.h"

PhysicsBox::PhysicsBox(btVector3 size, btVector3 position, btScalar mass)
{
	this->shape = new btBoxShape(size);
	btTransform transform;
	transform.setIdentity();

	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		this->shape->calculateLocalInertia(mass, localInertia);
	}

	transform.setOrigin(position);

	this->motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, this->motionState, this->shape, localInertia);
	this->body = new btRigidBody(info);
}

btVector3 PhysicsBox::getPosition()
{
	btTransform transform;
	this->body->getMotionState()->getWorldTransform(transform);
	return transform.getOrigin();
}

btCollisionShape* PhysicsBox::getShape() 
{ 
	return this->shape; 
}

btRigidBody* PhysicsBox::getBody() 
{ 
	return this->body; 
}