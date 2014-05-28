#ifndef LIGHT_H
#define LIGHT_H

#include "Definitions.h"
#include "MovableObject.h"

class Light : public MovableObject
{
public:
	Light(std::string name, SceneManager* newSceneManager);
	~Light(void);

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::vec3 parentOrient);

	void setPosition(glm::vec3 newPos);

	glm::vec3 getPosition()
	{
		return mPosition;
	}

private:
	glm::vec3 mPosition;
	glm::vec3 mOrientation; //for directional and spot

	/*
	//inheritated
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
	bool mAttachedToNode;*/
};

#endif