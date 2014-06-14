#include "SceneManager.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "Material.h"

SceneManager::SceneManager(Renderer* mCurrentRenderer)
{
	mRenderer = mCurrentRenderer;
	mRootSceneNode = new SceneNode("RootNode", this);
}


SceneManager::~SceneManager(void)
{
	//Memory deallocation handeled by the resoruceManager
	mShaderMap.clear();
	mTextureMap.clear();
	mMaterialMap.clear();

	tEntityMap::iterator entityIterator;
	for(entityIterator = mEntityMap.begin(); entityIterator != mEntityMap.end(); ++entityIterator)
	{
		delete entityIterator->second;
	}
	mEntityMap.clear();

	tCameraMap::iterator cameraIterator;
	for(cameraIterator = mCameraMap.begin(); cameraIterator != mCameraMap.end(); ++cameraIterator)
	{
		delete cameraIterator->second;
	}
	mCameraMap.clear();

	tLightMap::iterator lightIterator;
	for(lightIterator = mLightMap.begin(); lightIterator != mLightMap.end(); ++lightIterator)
	{
		delete lightIterator->second;
	}
	mLightMap.clear();

	tFrameBufferMap::iterator fboIterator;
	for (fboIterator = mFrameBufferMap.begin(); fboIterator != mFrameBufferMap.end(); ++fboIterator)
	{
		delete fboIterator->second;
	}
	mFrameBufferMap.clear();
}


Shader* SceneManager::createShader(std::string shaderName, std::string shaderPath)
{
	//get ResourceManager singleton pointer
	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr();

	//iterate through the map in search of the shader
	tShaderMap::iterator it = mShaderMap.find(shaderName);
	if(it != mShaderMap.end())
	{
		return it->second;
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
	tShaderMap::iterator it = mShaderMap.find(shaderName);

	if(it != mShaderMap.end())
	{
		mShaderMap.erase(it);
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

Texture* SceneManager::createTexture(std::string textureName, bool mipmap, GLint format, std::string texturePath, int width, int height)
{
	//get ResourceManager singleton pointer
	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr();

	//iterate through the map in search of the texture
	tTextureMap::iterator it = mTextureMap.find(textureName);
	//if it exist return it
	if(it != mTextureMap.end())
	{
		return it->second;
	}

	Texture* mTexture;
	//if the texture doesnt exist
	if (texturePath != "NULL")
	{
		//load it into disc (if its not already loaded
		mTexture = mResourceManager->loadTexture(textureName, mipmap, format, texturePath);
	}
	else
	{
		mTexture = mResourceManager->createTexture(textureName, width, height, mipmap, format);
	}

	//save it into the texture map
	mTextureMap.insert(std::pair<std::string, Texture*>(textureName, mTexture));

	//return the texture
	return mTexture;
}

void SceneManager::deleteTexture(std::string textureName)
{
	//iterate through the map in search of the texture
	tTextureMap::iterator it = mTextureMap.find(textureName);
	//if it exist erase it
	if(it != mTextureMap.end())
	{
		mTextureMap.erase(it);
	}
}

//creates a new entity
Entity* SceneManager::createEntity(std::string entityName, std::string meshPath)
{
	//search if an entity with that name already exists
	tEntityMap::iterator it = mEntityMap.find(entityName);
	if(it != mEntityMap.end())
	{
		return it->second;
	}

	//else
	//create the entity 
	Entity* mEntity;
	if(meshPath != "NULL")
	{
		mEntity = new Entity(entityName, meshPath, this);
	}
	else
	{
		mEntity = new Entity(entityName, this);
	}

	//save it into the texture map
	mEntityMap.insert(std::pair<std::string, Entity*>(entityName, mEntity));

	//return it
	return mEntity;
}
//delete an entity by name (from scene)
void SceneManager::deleteEntity(std::string entityName)
{
	tEntityMap::iterator it = mEntityMap.find(entityName);
	if(it != mEntityMap.end())
	{
		delete it->second;
		mEntityMap.erase(it);
	}
}


Light* SceneManager::createLight(std::string lightName)
{
	tLightMap::iterator it = mLightMap.find(lightName);

	if(it != mLightMap.end())
	{
		return it->second;
	}

	//if not found create it
	Light* newLight = new Light(lightName, this);
	//Attach the light to the sceneNode, just for lights and cameras
	mRootSceneNode->attachObject(newLight);

	mLightMap.insert(std::pair<std::string, Light*>(lightName, newLight));

	return newLight;
}

void SceneManager::deleteLight(std::string lightName)
{
	tLightMap::iterator it = mLightMap.find(lightName);

	if(it != mLightMap.end())
	{
		delete it->second;
		mLightMap.erase(it);
	}
}

Light* SceneManager::getLight(std::string lightName)
{
	tLightMap::iterator it = mLightMap.find(lightName);

	if(it != mLightMap.end())
	{
		return it->second;
	}

	//if not found
	return NULL;
}


Camera* SceneManager::createCamera(std::string cameraName)
{
	tCameraMap::iterator it = mCameraMap.find(cameraName);
	//if the camera already exists
	if(it != mCameraMap.end())
	{
		return it->second;
	}

	//else
	//create a new camera
	Camera* newCamera = new Camera(cameraName, this);
	//Attach it to the root SceneNode (just for cameras and lights)
	mRootSceneNode->attachObject(newCamera);

	//save to the camera map
	mCameraMap.insert(std::pair<std::string, Camera*>(cameraName, newCamera));

	//set the new camera to be current
	mCurrentCamera = newCamera;
	//return the camera
	return newCamera;
}

void SceneManager::deleteCamera(std::string cameraName)
{
	tCameraMap::iterator it = mCameraMap.find(cameraName);
	//if the camera already exists
	if(it != mCameraMap.end())
	{
		//if the camera to be deleted is the current camera set it to null
		if(mCurrentCamera->getName() == cameraName)
		{
			mCurrentCamera = NULL;
		}

		delete it->second;

		mCameraMap.erase(it);
	}
}

void SceneManager::setCurrentCamera(std::string newCamera)
{	
	tCameraMap::iterator it = mCameraMap.find(newCamera);
	//if the camera already exists
	if(it != mCameraMap.end())
	{
		mCurrentCamera = it->second;
	}
}
void SceneManager::setCurrentCamera(Camera* newCamera)
{
	mCurrentCamera = newCamera;
}

void SceneManager::processViewMatrix()
{
	//compute the new camera matrix
	glm::vec3 pos, orient;
	pos = mCurrentCamera->mParentSceneNode->getDerivedPosition();
	orient = mCurrentCamera->mParentSceneNode->getDerivedOrientation();

	mCurrentCamera->process(glm::mat4(1.0), glm::mat4(1.0), pos, orient);
}

glm::mat4 SceneManager::getViewMatrix()
{
	assert(mCurrentCamera);

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


Material* SceneManager::createMaterial(std::string materialName, std::string materialPath)
{
	tMaterialMap::iterator it = mMaterialMap.find(materialName);

	if (it != mMaterialMap.end())
	{
		return it->second;
	}

	Material* mat;

	if (materialPath != "NULL")
	{
		mat = ResourceManager::getSingletonPtr()->loadMaterial(materialName, materialPath, this);
	}
	else
	{
		mat = ResourceManager::getSingletonPtr()->createMaterial(materialName, this);
	}

	mMaterialMap.insert(std::pair<std::string, Material*>(materialName, mat));

	return mat;
}

Material* SceneManager::getMaterial(std::string materialName)
{
	tMaterialMap::iterator it = mMaterialMap.find(materialName);

	if (it != mMaterialMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

FrameBuffer* SceneManager::createFrameBuffer(std::string name, int width, int height)
{
	tFrameBufferMap::iterator it = mFrameBufferMap.find(name);

	if (it != mFrameBufferMap.end())
	{
		return it->second;
	}
	else
	{
		FrameBuffer* newFBO = new FrameBuffer(name, width, height, this);

		mFrameBufferMap.insert(std::pair<std::string, FrameBuffer*>(name, newFBO));

		return newFBO;
	}
}

void SceneManager::deleteFrameBuffer(std::string name)
{
	tFrameBufferMap::iterator it = mFrameBufferMap.find(name);

	if (it != mFrameBufferMap.end())
	{
		delete it->second;

		mFrameBufferMap.erase(it);
	}
}

FrameBuffer* SceneManager::getFrameBuffer(std::string name)
{
	tFrameBufferMap::iterator it = mFrameBufferMap.find(name);

	if (it != mFrameBufferMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}