#include "Light.h"

#include "SceneNode.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Entity.h"
#include "RigidBody.h"

Light::Light(std::string name, SceneManager* newSceneManager)
{
	mName= name;
	mSceneManager = newSceneManager;

	mType = POINTLIGHT;
	mPosition = glm::vec3(0.0, 0.0, 0.0);
	mDirection = glm::vec3(0.0, -1.0, 0.0);
	mColor = glm::vec3(1.0, 0.0, 0.0);
	mRadius = 1.0;
	mAttenuationRadius = 2.0;
	mOuterAngle = 30.0;
	mParentSceneNode = NULL;
	mAttachedToNode = false;
	visible = true;
	castShadows = true;
	drawAABB = false;

	mTypeOfMovableObject = tTypeEnum::LIGHT;

	mDrawNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name + "light_draw_node");
	mDrawNode->setScale(glm::vec3(0.05));
	mDrawEntity = mSceneManager->createEntity(name + "_light_draw_entity", "sphere.obj");
	mDrawNode->attachObject(mDrawEntity);
	mDrawEntity->setVisible(false);
	//mDrawEntity->setPhysicsOn(false);
	mDrawEntity->setRayCastReturnPointer(this);
	mDrawEntity->setMass(0.0, false); //Set kinetic (so we can move it in the editor without problems)
}

Light::~Light(void)
{
}


void Light::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient)
{
	glm::vec3 mDerivedPos = parentPos + mPosition;
	glm::vec3 mDerivedDirection = glm::eulerAngles(parentOrient) + mDirection;

	switch (mType)
	{
		case POINTLIGHT:
		{
			//Calculate 
			//Send uniforms
						   mSceneManager->getCurrentShader()->Uniform("lightType", mType);
						   mSceneManager->getCurrentShader()->Uniform("lightPos", mDerivedPos);
						   mSceneManager->getCurrentShader()->Uniform("lightRadius", mRadius);
						   mSceneManager->getCurrentShader()->Uniform("lightColor", mColor);
			break;
		}
		case DIRECTIONALLIGHT:
		{
							//Send uniforms
							mSceneManager->getCurrentShader()->Uniform("lightType", mType);
							mSceneManager->getCurrentShader()->Uniform("lightPos", mDerivedPos);
							mSceneManager->getCurrentShader()->Uniform("lightColor", mColor);
							mSceneManager->getCurrentShader()->Uniform("lightDirection", mDerivedDirection);
			break;
		}
		case SPOTLIGHT:
		{
							//Send uniforms
							mSceneManager->getCurrentShader()->Uniform("lightType", mType);
							mSceneManager->getCurrentShader()->Uniform("lightPos", mDerivedPos);
							mSceneManager->getCurrentShader()->Uniform("lightColor", mColor);
							mSceneManager->getCurrentShader()->Uniform("lightDirection", mDerivedDirection);
							mSceneManager->getCurrentShader()->Uniform("lightCutOff", glm::cos(glm::radians(mOuterAngle)));
							mSceneManager->getCurrentShader()->Uniform("lightAttenuationRadius", mAttenuationRadius);
			break;
		}
	}
	
	if (mSceneManager->isEditorModeOn())
	{
		mDrawEntity->setVisible(true);
		mDrawNode->setPosition(getPosition());
		mDrawEntity->attachMaterial(mName + "_light_drawentity_mat");
		mDrawEntity->setColor(mColor);
		mDrawEntity->getRigidBody()->setShape_Sphere(mRadius);

	}
	else
	{
		mDrawEntity->setVisible(false);
	}
}

glm::vec3 Light::getPosition()
{
	if (mParentSceneNode == mSceneManager->getRootSceneNode())
	{
		return mPosition;
	}
	else
	{
		return mParentSceneNode->getDerivedPosition();
	}
}

void Light::translate(glm::vec3 trans)
{
	mPosition += trans * mSceneManager->mDeltaTime;
}

void Light::setPosition(glm::vec3 newPos)
{
	mPosition = newPos;
}

void Light::setDirection(glm::vec3 newDir)
{
	mDirection = newDir;
}

void Light::setColor(glm::vec3 newColor)
{
	mColor = newColor;
}

void Light::setRadius(real newRadius)
{
	mRadius = newRadius;
}

void Light::setType(tLightTypeEnum newType)
{
	mType = newType;
}

void Light::setAttenuationRadius(real nRadius)
{
	mAttenuationRadius = nRadius;
}