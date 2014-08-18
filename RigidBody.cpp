#include "RigidBody.h"

#include "Mesh.h"
#include "SceneNode.h"
#include "Entity.h"

RigidBody::RigidBody(std::string name, btDiscreteDynamicsWorld* theWorld)
{
	mName = name;
	mCurrentDynamicWorld = theWorld;

	mBoxDimensions = glm::vec3(0.0);
	mRadius = 0.0;
	mNormal = glm::vec3(0.0);
	mOrigin = 0.0;

	mBulletCollisionShape = nullptr;
	mBulletMotionState = nullptr;
	mBulletRigidBody = nullptr;
}


RigidBody::~RigidBody()
{
	delete mBulletCollisionShape;
}

void RigidBody::setUpRigidBody(real mass, SceneNode* node, Entity* ent)
{
	glm::quat orientation = node->getDerivedOrientation();
	glm::vec3 position = node->getDerivedPosition();

	//If we dont have collision shape
	if (mBulletCollisionShape == nullptr)
	{
		std::cout << "Error: Cant set up rigidBody without a collision shap - " << mName << std::endl;
		assert(mBulletCollisionShape != nullptr);
	}
	//If we already have one
	if (mBulletMotionState != nullptr)
	{
		delete mBulletMotionState;
	}
	mBulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z)));

	//If the object is dynamic calculate its inertia
	btVector3 localInertia(0.0, 0.0, 0.0);
	if (mass != 0)
	{
		mBulletCollisionShape->calculateLocalInertia(mass, localInertia);
	}

	btRigidBody::btRigidBodyConstructionInfo mBulletRigidBodyCI(
		mass, //Mass in kg
		mBulletMotionState,
		mBulletCollisionShape,
		localInertia //Local inertia
		);

	//If we already have one
	if (mBulletRigidBody != nullptr)
	{
		delete mBulletRigidBody;
	}

	//Create the rigidbody and set its entity pointer
	mBulletRigidBody = new btRigidBody(mBulletRigidBodyCI);
	mBulletRigidBody->setUserPointer(ent);

	mCurrentDynamicWorld->addRigidBody(mBulletRigidBody);
}


void RigidBody::setCollisionShape_Box(glm::vec3& boxDimensions)
{
	//Delete old collision shape
	if (mBulletCollisionShape != nullptr)
	{
		delete mBulletCollisionShape;
	}

	mBulletCollisionShape = new btBoxShape(btVector3(boxDimensions.x / 2.0, boxDimensions.y / 2.0, boxDimensions.z / 2.0));
	mBoxDimensions = boxDimensions;
}
void RigidBody::setCollisionShape_Sphere(real radius)
{
	//Delete old collision shape
	if (mBulletCollisionShape != nullptr)
	{
		delete mBulletCollisionShape;
	}

	mBulletCollisionShape = new btSphereShape(radius);
	mRadius = radius;
}
void RigidBody::setCollisionShape_Plane(glm::vec3& normal, real origin)
{
	//Delete old collision shape
	if (mBulletCollisionShape != nullptr)
	{
		delete mBulletCollisionShape;
	}

	mBulletCollisionShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), origin);
	mNormal = normal;
	mOrigin = origin;
}

void RigidBody::setCollisionShape_ConvexHull(Mesh* model)
{
	//Delete old collision shape
	if (mBulletCollisionShape != nullptr)
	{
		delete mBulletCollisionShape;
	}

	btConvexHullShape newShape;

	for (int i = 0; i < model->mMeshComponentsVector.size(); ++i)
	{
		for (int j = 0; j < model->mMeshComponentsVector[i].mVertexVector.size(); j += 3)
		{
			newShape.addPoint(btVector3(model->mMeshComponentsVector[j].mVertexVector.at(j), model->mMeshComponentsVector[j].mVertexVector.at(j + 1), model->mMeshComponentsVector[j].mVertexVector.at(j + 2)));
		}
	}

	mBulletCollisionShape = new btConvexHullShape(newShape);
	
}
