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
	//delete
	//delete the shader by name (from the scene)
	void deleteShader(std::string shaderName);
	//return that shader by name
	Shader* getShader(std::string shaderName);
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
	//Creates a new texture, It loads the texture if a texturePath is selected (dont need to set the dimensions of the texture), if not it creates an empty texture with the desired width and heigh
	Texture* createTexture(std::string textureName, bool mipmap, GLint format, std::string texturePath, int width = 0, int height = 0);
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
	void processLights(); //Creates the specific polygon for each light, calculate things and send uniforms (deferred shading)
	int getNumOfLights()
	{
		return mLightMap.size();
	}

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

	//Material
	Material* createMaterial(std::string materialName, std::string materialPath);
	Material* getMaterial(std::string materialName);
	//void deleteMaterial(std::string materialName); need to be implemented (also in resourceManager)

	//FrameBuffer
	FrameBuffer* createFrameBuffer(std::string name, int width, int height);
	void deleteFrameBuffer(std::string name);
	FrameBuffer* getFrameBuffer(std::string name);

	//aux
	//sets the perspectiveMAtrix
	void setPerspectiveMatrix(real FOV, real width, real height, real zNear, real zFar = 0);
	//return the perspective matrix
	glm::mat4 getProjectionMatrix()
	{
		return mProjectionMatrix;
	}
	//return the rootSceneNode
	SceneNode* getRootSceneNode()
	{
		return mRootSceneNode;
	}
	//return the world position of the current mouse position
	glm::vec2 getMousePosition_NDC();
	glm::vec3 getMousePosition_WorldSpace();

	//UI
	void renderUI();
	UIDisplayer* createUIDisplayer(std::string name);
	void deleteUIDisplayer(std::string name);
	UIDisplayer* getUIDisplayer(std::string name);
	void setCurrentUIDisplayer(std::string name);

	//Timer
	real mDeltaTime;

private:

	//maps
	typedef std::map<std::string, Shader*> tShaderMap;
	typedef std::map<std::string, Texture*> tTextureMap;
	typedef std::map<std::string, Entity*> tEntityMap;
	typedef std::map<std::string, Camera*> tCameraMap;
	typedef std::map<std::string, Light*> tLightMap;
	typedef std::map<std::string, Material*> tMaterialMap;
	typedef std::map<std::string, FrameBuffer*> tFrameBufferMap;
	typedef std::map<std::string, UIDisplayer*> tUIDisplayersMap;

	//define maps
	tShaderMap mShaderMap;
	tTextureMap mTextureMap;
	tEntityMap mEntityMap;
	tCameraMap mCameraMap;
	tLightMap mLightMap;
	tMaterialMap mMaterialMap;
	tFrameBufferMap mFrameBufferMap;
	tUIDisplayersMap mUiDisplayersMap;

	//projection matrix
	glm::mat4 mProjectionMatrix;

	//root sceneNode
	SceneNode* mRootSceneNode;

	//shader private members-----
	//current binded Shader
	Shader* mCurrentShader;

	//camera private members
	//current active camera
	Camera* mCurrentCamera;

	//UI private members
	//current active displayer
	UIDisplayer* mCurrentUIDisplayer;

	//Pointer to renderer
	Renderer* mRenderer;
};

#endif