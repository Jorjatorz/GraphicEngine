#include "Light.h"

#include "SceneNode.h"
#include "SceneManager.h"
#include "Shader.h"

Light::Light(std::string name, SceneManager* newSceneManager)
{
	mName= name;
	mSceneManager = newSceneManager;

	mType = POINTLIGHT;
	mPosition = glm::vec3(0.0, 0.0, 0.0);
	mOrientation = glm::vec3(0.0, 0.0, 0.0);
	mColor = glm::vec3(1.0, 0.0, 0.0);
	mSize = 1.0;
	mParentSceneNode = NULL;
	mAttachedToNode = false;
	visible = true;
	castShadows = true;

	mTypeOfMovableObject = tTypeEnum::Entity;
}

Light::~Light(void)
{
}


void Light::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::vec3 parentOrient)
{
	glm::vec3 mDerivedPos = parentPos + mPosition;
	glm::vec3 mDerivedOrientation = parentOrient + mOrientation;

	switch (mType)
	{
		case POINTLIGHT:
		{
			//Calculate 
			//Send uniforms
						   mSceneManager->getCurrentShader()->Uniform("lightType", mType);
						   mSceneManager->getCurrentShader()->Uniform("lightPos", mDerivedPos);
						   mSceneManager->getCurrentShader()->Uniform("lightColor", mColor);
			break;
		}
		case DIRECTIONALLIGHT:
		{
			break;
		}
		case SPOTLIGHT:
		{
			break;
		}
	}
	//send uniforms
}

void Light::setPosition(glm::vec3 newPos)
{
	mPosition = newPos;
}

void Light::setColor(glm::vec3 newColor)
{
	mColor = newColor;
}