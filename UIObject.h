#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "Definitions.h"

#include <string>
//CLASS FOR INHERITANCE OF UI ELEMENTS
class UIObject
{
public:
	UIObject();
	~UIObject();

	void drawObject(Shader* UIShader);

	//Rendering
	void setColor(glm::vec3& newColor);
	glm::vec3 getColor()
	{
		return glm::vec3(mColor);
	}
	void setTexture(Texture* newTex);
	bool isTextured();

	//Transformations
	void setPosition(glm::vec2& pos);
	glm::vec2 getPosition()
	{
		return glm::vec2(mPosition);
	}
	void setSize(glm::vec2& size);
	glm::vec2 getSize()
	{
		return glm::vec2(mSize);
	}

	//Others
	std::string getName()
	{
		return mName;
	}
	bool rayTestToObject(glm::vec2 rayCoords);
	void setSelected(bool sel);

protected:
	std::string mName;
	SceneManager* mCurrentSceneManager;

	//Special properties
	bool mSelected; //true if the windows is selected
	bool movable_;

	//Rendering properties
	bool visible_;
	glm::vec4 mColor; //Alpha included
	Texture* mTexture; //Just if its textured

	//Transformation properties
	glm::vec3 mPosition, mParentPosition; //Parent position not for Windows - From [-1:1]
	glm::vec3 mSize, mParentSize; //Parent size not for Windows
	glm::mat4 mTransformMatrix;
	void setTransforms();

	//Others
	void sendUniforms(Shader* UIShader);
	virtual void update() = 0;


	//OpenGL properties
	GLuint glVBO, glVAO;
	//OpenGL funcs
	void generateBuffers();

};

#endif