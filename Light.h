#ifndef LIGHT_H
#define LIGHT_H

#include "Definitions.h"
#include "MovableObject.h"

class Light : public MovableObject
{
public:
	Light(std::string name, SceneManager* newSceneManager);
	~Light(void);

	typedef enum
	{
		POINTLIGHT,
		DIRECTIONALLIGHT,
		SPOTLIGHT
	} tLightTypeEnum;

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::vec3 parentOrient);

	void setPosition(glm::vec3 newPos);
	void setDirection(glm::vec3 newDir);
	void setColor(glm::vec3 newColor);
	void setRadius(float newRadius);
	void setType(tLightTypeEnum newType);
	//Set orientation, color, size...

	glm::vec3 getPosition()
	{
		return mPosition;
	}
	glm::vec3 getColor()
	{
		return mColor;
	}
	glm::vec3 getDirection()
	{
		return mDirection;
	}
	float getRadius()
	{
		return mRadius;
	}
	tLightTypeEnum getType()
	{
		return mType;
	}

private:
	tLightTypeEnum mType;
	glm::vec3 mPosition;
	glm::vec3 mDirection; //for directional and spot
	glm::vec3 mColor;
	float mRadius; //pointlight
	float mCutOff; //Spotlight

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