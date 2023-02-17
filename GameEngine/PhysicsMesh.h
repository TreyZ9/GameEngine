#pragma once

#include <bullet3/btBulletDynamicsCommon.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class PhysicsMesh
{
private:
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btRigidBody* body;

public:
	std::vector<Mesh> meshes;

	PhysicsMesh() = default;
	PhysicsMesh(const std::string& filename, const btVector3& position);
	~PhysicsMesh() = default;

	btVector3 getPosition();

	btCollisionShape* getShape();
	btRigidBody* getBody();
};
