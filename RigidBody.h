#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Definitions.h"

#include <string>

class RigidBody
{
public:
	RigidBody(std::string name, btDiscreteDynamicsWorld* theWorld);
	~RigidBody();

	void setUpRigidBody(real mass, SceneNode* node, Entity* ent);

	void setCollisionShape_Box(glm::vec3& boxDimensions);
	void setCollisionShape_Sphere(real radius);
	void setCollisionShape_Plane(glm::vec3& normal, real origin);
	void setCollisionShape_ConvexHull(Mesh* model);

private:
	std::string mName;

	//for box shape
	glm::vec3 mBoxDimensions;
	//for sphere shape
	real mRadius;
	//for plane shape
	glm::vec3 mNormal;
	real mOrigin;

	//Bullet staff
	btDiscreteDynamicsWorld* mCurrentDynamicWorld; //Ptr to the dynamic world
	btCollisionShape* mBulletCollisionShape;
	btDefaultMotionState* mBulletMotionState;
	btRigidBody* mBulletRigidBody;
};

#endif