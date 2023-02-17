#include "PhysicsMesh.h"

#include "Vertex.h"
#include "Model.h"

PhysicsMesh::PhysicsMesh(const std::string& filename, const btVector3& position)
{
	Model model(filename);
	Mesh mesh = model.meshes[0];

	btTriangleMesh* triMesh = new btTriangleMesh();
	for (int i = 0; i < mesh.indices.size(); i += 3)
	{
		Vertex v1 = mesh.vertices[mesh.indices[i]];
		Vertex v2 = mesh.vertices[mesh.indices[i+1]];
		Vertex v3 = mesh.vertices[mesh.indices[i+2]];

		btVector3 a(v1.Position.x, v1.Position.y, v1.Position.z);
		btVector3 b(v2.Position.x, v2.Position.y, v2.Position.z);
		btVector3 c(v3.Position.x, v3.Position.y, v3.Position.z);

		triMesh->addTriangle(a, b, c);
	}

	this->shape = new btBvhTriangleMeshShape(triMesh, true);
	// this->shape->setMargin(0.01f);

	btTransform transform;
	transform.setIdentity();
	
	btVector3 localInertia(0, 0, 0);

	transform.setOrigin(position);
	
	this->motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(btScalar(0.0f), this->motionState, this->shape, localInertia);
	this->body = new btRigidBody(info);
	
}

btVector3 PhysicsMesh::getPosition()
{
	btTransform transform;
	this->body->getMotionState()->getWorldTransform(transform);
	return transform.getOrigin();
}

btCollisionShape* PhysicsMesh::getShape() 
{ 
	return this->shape; 
}

btRigidBody* PhysicsMesh::getBody() 
{ 
	return this->body; 
}