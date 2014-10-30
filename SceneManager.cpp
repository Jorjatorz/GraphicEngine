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
#include "Math.h"
#include "InputManager.h"
#include "Window.h"
#include "Root.h"
#include "UIDisplayer.h"
#include "UIManager.h"
#include "WorldEditor.h"

SceneManager::SceneManager(Renderer* mCurrentRenderer)
{
	mRenderer = mCurrentRenderer;
	mRootSceneNode = new SceneNode("RootNode", NULL, this);

	//Init pointers
	mCurrentCamera = NULL;
	mCurrentShader = NULL;
	mCurrentDisplayer = NULL;
	mWorldEditor = new WorldEditor(this);

	editorModeOn_ = true;

	//Create a screenQuad for FBOS
	ResourceManager::getSingletonPtr()->createScreenQuad();
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

	delete mWorldEditor;
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

Shader* SceneManager::getShader(std::string shaderName)
{
	//iterate through the map in search of the shader
	tShaderMap::iterator it = mShaderMap.find(shaderName);
	if (it != mShaderMap.end())
	{
		return it->second;
	}

	return NULL;
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
		mTexture = mResourceManager->loadTexture(texturePath, mipmap, format);
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
		//First delete it from node
		it->second->getAttachedSceneNode()->detachObject(it->second);
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

void SceneManager::processLights()
{

	//Set read FBO
	FrameBuffer* readFBO = getFrameBuffer("deferredFBO");
	readFBO->bindForReading();
	//Set write FBO
	FrameBuffer* writeFBO = getFrameBuffer("lightFBO");
	writeFBO->bindForDrawing();

	//Opengl functions
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//Set the light shader
	Shader* lightShader = createShader("lightPassShader", "lightPass");
	bindShader(lightShader);

	//Send textures
	lightShader->UniformTexture("diffuseTex", 0);
	lightShader->UniformTexture("normalTex", 1);
	lightShader->UniformTexture("depthTex", 2);
	//Other uniforms
	lightShader->Uniform("cameraPos", mCurrentCamera->getPosition());
	
	//Process all the lights
	tLightMap::iterator it;
	for (it = mLightMap.begin(); it != mLightMap.end(); ++it)
	{

		//Set the lightvolume for each type of light
		Entity* lightVolume;
		switch (it->second->getType())
		{
		case Light::POINTLIGHT:
		{
								  lightVolume = createEntity(it->first + "LightVolume", "sphere.obj");
								  break;
		}
		case Light::SPOTLIGHT:
		{
								 lightVolume = createEntity(it->first + "LightVolume", "cone.obj");
								 glm::mat4 mat = glm::mat4(1.0); //We modify the model matrix so we rotate over the upper cone vertex
								 mat = glm::translate(mat, glm::vec3(0.0, -1.0, 0.0));
								 lightVolume->setModelMatrix(mat);
								 break;
		}
		default:
			lightVolume = NULL;
			break;
		}

		//Matrices
		glm::mat4 V = getViewMatrix();

		//Transform the lightVolume
		glm::mat4 transM = glm::mat4(1.0);

		transM = glm::translate(transM, it->second->getPosition()); //First translate it
		//Rotate it if Spotlight
		if (it->second->getType() == Light::SPOTLIGHT)
		{
			Math aux;
			transM = transM * aux.lookAt(it->second->getDirection(), it->second->getPosition(), glm::vec3(0.0, -1.0, 0.0), true); 
		}
		//Scale it
		if (it->second->getType() == Light::SPOTLIGHT)
		{
			glm::vec3 a = glm::vec3(0.0, 1.0, 0.0) * (it->second->getAttenuationRadius() * 0.5f);
			glm::vec3 b = glm::vec3(1.0, 0.0, 1.0) * (it->second->getAttenuationRadius() * glm::tan(glm::radians(it->second->getOuterAngle())));

			transM = glm::scale(transM, a + b);
		}
		else
		{
			transM = glm::scale(transM, glm::vec3(it->second->getRadius()));
		}
		//Compute final information for rendering
		glm::mat4 PVS = mProjectionMatrix * V * transM; //Perspective * View * Trans
		glm::vec3 pos = it->second->mParentSceneNode->getDerivedPosition();
		glm::quat orient = it->second->mParentSceneNode->getDerivedOrientation();

		//Process the light (Send uniforms)
		it->second->process(PVS, V, pos, orient);

		//Process its light volume
		if (it->second->getType() != Light::DIRECTIONALLIGHT)
		{
			lightVolume->attachMaterial("lightVolumeMaterial");
			lightVolume->getMaterial()->setShader(lightShader);
			lightVolume->getMaterial()->setBaseColorV(it->second->getColor());
			lightVolume->process(PVS, V);
		}
		else
		{
			lightShader->UniformMatrix("MVP", glm::mat4(1.0));

			//We dont bind the shader because is already binded
			glBindVertexArray(ResourceManager::getSingletonPtr()->getScreenQuadVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	unbindCurrentShader();
	writeFBO->unBind();
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
	glm::vec3 pos;
	glm::quat orient;
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
		mProjectionMatrix = glm::perspective(FOV, width/height, zNear, zFar);
	}
	else
	{
		mProjectionMatrix = glm::infinitePerspective(FOV, width/height, zNear);
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

glm::vec2 SceneManager::getMousePosition_NDC()
{
	int mouseX, mouseY;
	InputManager::getSingletonPtr()->getMousePosition(mouseX, mouseY);
	int windowH, windowW;
	windowW = mRenderer->getCurrentWindow()->getWidth();
	windowH = mRenderer->getCurrentWindow()->getHeight();


	real x = (2.0f * mouseX) / windowW - 1.0f;
	real y = 1.0f - (2.0f * mouseY) / windowH;
	
	return glm::vec2(x, y);
}

glm::vec3 SceneManager::getMousePosition_WorldSpace()
{
	int mouseX, mouseY;
	InputManager::getSingletonPtr()->getMousePosition(mouseX, mouseY);
	int windowH, windowW;
	windowW = mRenderer->getCurrentWindow()->getWidth();
	windowH = mRenderer->getCurrentWindow()->getHeight();


	real x = (2.0f * mouseX) / windowW - 1.0f;
	real y = 1.0f - (2.0f * mouseY) / windowH;
	real z = 1.0f;
	glm::vec3 dir_NDS = glm::vec3(x, y, z);

	glm::vec4 dir_clip = glm::vec4(dir_NDS.x, dir_NDS.y, -1.0, 1.0);
	glm::vec4 dir_eye = glm::inverse(mProjectionMatrix) * dir_clip;
	dir_eye = glm::vec4(dir_eye.x, dir_eye.y, -1.0, 0.0);

	glm::vec3 dir_wor = glm::vec3(glm::inverse(getViewMatrix()) * dir_eye);
	dir_wor = glm::normalize(dir_wor);

	return dir_wor;
}


glm::vec2 SceneManager::getMousePosition_WindowSDL()
{
	//Return the mouse position in SDL coords
	int mouseX, mouseY;
	InputManager::getSingletonPtr()->getMousePosition(mouseX, mouseY);

	return glm::vec2(mouseX, mouseY);
}

UIDisplayer* SceneManager::createDisplayer(std::string name)
{
	return Root::getSingletonPtr()->mUIManager->createDisplayer(name, this);
}
void SceneManager::deleteDisplayer(std::string name)
{
	Root::getSingletonPtr()->mUIManager->deleteDisplayer(name);
}
UIDisplayer* SceneManager::getDisplayer(std::string name)
{
	return Root::getSingletonPtr()->mUIManager->getDisplayer(name);
}

void SceneManager::setCurrentDisplayer(std::string name)
{
	mCurrentDisplayer = getDisplayer(name);

	Root::getSingletonPtr()->mUIManager->setCurrentDisplayer(mCurrentDisplayer);
}

glm::vec2 SceneManager::getWindowDimensions()
{
	Window* wind = getRenderer()->getCurrentWindow();

	return glm::vec2(wind->getWidth(), wind->getHeight());
}

void SceneManager::injectMouseDown_WorldEditor()
{
	mWorldEditor->selectObject_RayCast(mCurrentCamera->getPosition(), getMousePosition_WorldSpace());
}

void SceneManager::processWorldEditor()
{
	mWorldEditor->processWorldEditor();
}