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
	virtual void drawChildrens(Shader* UIShader);

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
		return mPosition;
	}
	glm::vec2 getDerivedPosition()
	{
		return mParentPosition + mPosition;
	}
	void setSize(glm::vec2& size);
	glm::vec2 getSize()
	{
		return mSize;
	}

	//Others
	std::string getName()
	{
		return mName;
	}
	bool rayTestToObject(glm::vec2 rayCoords);
	void setSelected(bool sel);
	virtual void buttonDown(glm::vec2& mousePos)
	{
		return;
	}
	virtual void buttonUp(glm::vec2& mousePos)
	{
		return;
	}

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
	void renderFont();

	//Text properties
	std::string mText;
	glm::vec3 mTextColor;
	glm::vec2 mTextPosition;
	glm::vec2 mTextSize;

	//Freetype stuff
	GLuint fontBitmap_;
	GLuint fontVBO_;
	GLuint fontVAO_;

	//Transformation properties
	glm::vec2 mPosition, mParentPosition; //Parent position not for Windows - From [-1:1]
	glm::vec2 mSize, mParentSize; //Parent size not for Windows
	glm::mat4 mTransformMatrix;
	void setTransforms();

	//Others
	void sendUniforms(Shader* UIShader);
	bool mouseDown_;

	virtual void update() = 0;

	//OpenGL properties
	GLuint glVBO, glVAO;
	//OpenGL funcs
	void generateBuffers();

};

#endif