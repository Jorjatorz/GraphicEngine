#include "Light.h"


Light::Light(std::string name, SceneManager* newSceneManager)
{
	mName= name;
	mSceneManager = newSceneManager;

	mPosition = glm::vec3(0.0, 0.0, 0.0);
	mOrientation = glm::vec3(0.0, 0.0, 0.0);
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
	//send uniforms
}

void Light::setPosition(glm::vec3 newPos)
{
	mPosition = newPos;
}