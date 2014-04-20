#include "SceneManager.h"


#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Camera.h"

SceneManager::SceneManager(void)
{
	mRootSceneNode = new SceneNode("RootNode", this);
}


SceneManager::~SceneManager(void)
{
}


Shader* SceneManager::createShader(std::string shaderName, std::string shaderPath)
{
	//get ResourceManager singleton pointer
	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr();

	//iterate through the map in search of the shader
	tShaderMap::iterator shaderIterator;
	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		//if it exist return it
		if(shaderName == shaderIterator->first)
		{
			return shaderIterator->second;
		}
	}

	//if the shader doesnt exist
	//load it into disc (if its not already loaded
	Shader* mShader = mResourceManager->loadShader(shaderName, shaderPath);

	//save it into the shader map
	mShaderMap.insert(std::pair<std::string, Shader*>(shaderName, mShader));

	//return the shader
	return mShader;
}

void SceneManager::deleteShader(std::string shaderName)
{
	//iterate through the map in search of the shader
	tShaderMap::iterator shaderIterator;
	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		//if it exist delete it
		if(shaderName == shaderIterator->first)
		{
			mShaderMap.erase(shaderIterator);

			break;
		}
	}
}

void SceneManager::setCurrentShader(Shader* newShader)
{
	mCurrentShader = newShader;
}

void SceneManager::bindShader(Shader* mNewShader)
{
	mNewShader->bind();

	mCurrentShader = mNewShader;
}

void SceneManager::bindCurrentShader()
{
	//check that mCurrentSahder isnt NULL
	assert(mCurrentShader);

	bindShader(mCurrentShader);
}

void SceneManager::unbindCurrentShader()
{
	mCurrentShader->unBind();

	mCurrentShader = NULL;
}

void SceneManager::unbindShader()
{
	mCurrentShader->unBind();
}

Texture* SceneManager::createTexture(std::string textureName, bool mipmap, std::string texturePath)
{
	//get ResourceManager singleton pointer
	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr();

	//iterate through the map in search of the texture
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		//if it exist return it
		if(textureName == textureIterator->first)
		{
			return textureIterator->second;
		}
	}

	//if the texture doesnt exist
	//load it into disc (if its not already loaded
	Texture* mTexture = mResourceManager->loadTexture(textureName, mipmap, texturePath);

	//save it into the texture map
	mTextureMap.insert(std::pair<std::string, Texture*>(textureName, mTexture));

	//return the texture
	return mTexture;
}

void SceneManager::deleteTexture(std::string textureName)
{
	//iterate through the map in search of the shader
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		//if it exist delete it
		if(textureName == textureIterator->first)
		{
			mTextureMap.erase(textureIterator);

			break;
		}
	}
}

//creates a new entity
Entity* SceneManager::createEntity(std::string entityName, std::string meshPath)
{
	//search if an entity with that name already exists
	tEntityMap::iterator entityIterator;
	for(entityIterator = mEntityMap.begin(); entityIterator != mEntityMap.end(); ++entityIterator)
	{
		if(entityIterator->first == entityName)
		{
			return entityIterator->second;
		}
	}
	//else

	//create the entity 
	Entity* mEntity = new Entity(entityName, meshPath, this);

	//save it into the texture map
	mEntityMap.insert(std::pair<std::string, Entity*>(entityName, mEntity));

	//return it
	return mEntity;
}
//delete an entity by name (from scene)
void SceneManager::deleteEntity(std::string entityName)
{
	tEntityMap::iterator entityIterator;
	for(entityIterator = mEntityMap.begin(); entityIterator != mEntityMap.end(); ++entityIterator)
	{
		if(entityIterator->first == entityName)
		{
			mEntityMap.erase(entityIterator);

			break;
		}
	}
}

Camera* SceneManager::createCamera(std::string cameraName)
{
	tCameraMap::iterator it;

	//if the camera already exists
	for(it = mCameraMap.begin(); it != mCameraMap.end(); ++it)
	{
		if(it->first == cameraName)
		{
			return it->second;
		}
	}
	//else

	//create a new camera
	Camera* newCamera = new Camera(cameraName);

	//save to the camera map
	mCameraMap.insert(std::pair<std::string, Camera*>(cameraName, newCamera));

	//set the new camera to be current
	mCurrentCamera = newCamera;
	//return the camera
	return newCamera;
}

void SceneManager::deleteCamera(std::string cameraName)
{
	tCameraMap::iterator it;

	//if the camera already exists
	for(it = mCameraMap.begin(); it != mCameraMap.end(); ++it)
	{
		if(it->first == cameraName)
		{
			//if the camera to be deleted is the current camera set it to null
			if(mCurrentCamera->getName() == cameraName)
			{
				mCurrentCamera = NULL;
			}

			delete it->second;

			return;
		}
	}
}

void SceneManager::setCurrentCamera(std::string newCamera)
{
	
	tCameraMap::iterator it;

	//if the camera already exists
	for(it = mCameraMap.begin(); it != mCameraMap.end(); ++it)
	{
		if(it->first == newCamera)
		{
			mCurrentCamera = it->second;

			return;
		}
	}
}
void SceneManager::setCurrentCamera(Camera* newCamera)
{
	mCurrentCamera = newCamera;
}

glm::mat4 SceneManager::getViewMatrix()
{
	assert(mCurrentCamera);

	//compute the new camera matrix
	mCurrentCamera->updateCamera();

	return mCurrentCamera->getCameraMatrix();
}

void SceneManager::setPerspectiveMatrix(real FOV, real width, real height, real zNear, real zFar)
{
	if(zFar > 0)
	{
		mPerspectiveMatrix = glm::perspective(FOV, width/height, zNear, zFar);
	}
	else
	{
		mPerspectiveMatrix = glm::infinitePerspective(FOV, width/height, zNear);
	}
}