#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Definitions.h"

#include <string>
#include <map>

class SceneManager
{
public:
	SceneManager(Renderer* mCurrentRenderer);
	~SceneManager(void);

	Renderer* getRenderer()
	{
		return mRenderer;
	}

	//Shader
	//create, bind, unbind
	//Creates a new shader, if it already exists it return a pointer to it
	Shader* createShader(std::string shaderName, std::string shaderPath);
	//delete and delete from disc
	//delete the shader by name (from the scene)
	void deleteShader(std::string shaderName);
	//set the new currentSahder
	void setCurrentShader(Shader* newShader);
	//bind shader and set it to currentSahder
	void bindShader(Shader* mNewShader);
	void bindCurrentShader();
	Shader* getCurrentShader()
	{
		return mCurrentShader;
	}
	//unbind shader and set currentoShader to null
	void unbindCurrentShader();
	//unbind shader
	void unbindShader();

	//Texture
	//create, etc
	//Creates a new texture, if it already exists it return a pointer to it
	Texture* createTexture(std::string textureName, bool mipmap, std::string texturePath);
	//delete texture by name (from the scene)
	void deleteTexture(std::string textueName);

	//Entity
	//creates a new entity
	Entity* createEntity(std::string entityName, std::string meshPath = "NULL");
	//delete an entity by name (from scene)
	void deleteEntity(std::string entityName);


	//Light
	Light* createLight(std::string lightName);

	void deleteLight(std::string lightName);

	Light* getLight(std::string lightName);

	//Camera
	//creates a new camera
	Camera* createCamera(std::string cameraName);
	//deletes a camera
	void deleteCamera(std::string cameraName);
	//set a camera as current
	void setCurrentCamera(std::string newCamera);
	void setCurrentCamera(Camera* newCamera);
	//calculates the viewMatrix
	void processViewMatrix();
	//return the curent camera matrix
	glm::mat4 getViewMatrix();
	//Return the current camera
	Camera* getCurrentCamera()
	{
		return mCurrentCamera;
	}

	//aux
	//sets the perspectiveMAtrix
	void setPerspectiveMatrix(real FOV, real width, real height, real zNear, real zFar = 0);
	//return the perspective matrix
	glm::mat4 getPerspectiveMatrix()
	{
		return mPerspectiveMatrix;
	}
	//return the rootSceneNode
	SceneNode* getRootSceneNode()
	{
		return mRootSceneNode;
	}

	//Timer
	real mDeltaTime;

private:

	//maps
	typedef std::map<std::string, Shader*> tShaderMap;
	typedef std::map<std::string, Texture*> tTextureMap;
	typedef std::map<std::string, Entity*> tEntityMap;
	typedef std::map<std::string, Camera*> tCameraMap;
	typedef std::map<std::string, Light*> tLightMap;

	//define maps
	tShaderMap mShaderMap;
	tTextureMap mTextureMap;
	tEntityMap mEntityMap;
	tCameraMap mCameraMap;
	tLightMap mLightMap;

	//perspective matrix
	glm::mat4 mPerspectiveMatrix;

	//root sceneNode
	SceneNode* mRootSceneNode;

	//shader private members-----
	//current binded Shader
	Shader* mCurrentShader;

	//camera private members
	//current active camera
	Camera* mCurrentCamera;

	//Pointer to renderer
	Renderer* mRenderer;
};

#endif