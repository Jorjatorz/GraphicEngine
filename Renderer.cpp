#include "Renderer.h"


#include "Root.h"
#include "SceneManager.h"
#include "FrameBuffer.h"
#include "Window.h"


Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
	delete mWindow;
}

void Renderer::createRenderer(std::string windowName, int width, int height, bool fullscreen)
{
	//inits components of SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//create a window
	mWindow = new Window(windowName, width, height, fullscreen);

	//init glew (after initializing the window
	glewExperimental = GL_TRUE;
	glewInit();

	initOpenGL();

	//create sceneManager
	Root* mRoot = Root::getSingletonPtr();
	mSceneManager = mRoot->createSceneManager("pruebas", this);

	//Create FBO
	FrameBuffer* fbo = mSceneManager->createFrameBuffer("deferredFBO", mWindow->getWidth(), mWindow->getHeight());
	FrameBuffer* fbo1 = mSceneManager->createFrameBuffer("lightFBO", mWindow->getWidth(), mWindow->getHeight());
	fbo->createGBuffer();
	fbo1->addTexture(GL_RGBA);
	fbo1->addTexture(GL_RGBA);

}

void Renderer::initOpenGL()
{
	//GET INFO
	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

	const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

	std::cout<<"OpenGL "<< version << " initialized (" << vendor << "; " << renderer << ")" << std::endl;
	std::cout<<"GLSL version: " << glslVersion << std::endl;

	//Set viewprot and clear color
	glViewport(0.0f, 0.0f, mWindow->getWidth(),mWindow->getHeight());
	glClearColor(0.5f, 0.5f ,0.5f ,1.0);

	//GL STATES AND ENABLES
	glEnable(GL_DEPTH_TEST);
}

//delete this
#include "Shader.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include <string>
#include <time.h>
#include "ResourceManager.h""
//
void Renderer::renderFrame(real deltaTime)
{
	//PRuebas
	mSceneManager->mDeltaTime = deltaTime;

	Entity* mEnt = mSceneManager->createEntity("cube", "guard.obj");
	Shader* mShader = mSceneManager->createShader("basic", "basic");
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("nod", glm::vec3(0.0, 0.0, -1.0));
	mSceneManager->setPerspectiveMatrix(60,  mWindow->getWidth(), mWindow->getHeight(), 0.01);
	Camera* mCam = mSceneManager->createCamera("camera1");
	//mCam->setControler(Camera::DEFAULT);
	mSceneManager->setCurrentShader(mShader);

	Entity* mEnt2 = mSceneManager->createEntity("ent2", "plane.obj");
	SceneNode* node2 = mSceneManager->getRootSceneNode()->createChildSceneNode("nod2", glm::vec3(2.0, -1.0, -1.0));
	//node2->rotate(glm::vec3(0.0, 1.0, 0.0), 1.0);
	node2->setPosition(glm::vec3(-1.0, -0.25, 0.0));
	node2->setScale(glm::vec3(2.0, 2.0, 2.0));
	node2->attachObject(mEnt2);
	node->attachObject(mEnt);
	glm::mat4 a = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.0, 0.0));
	mEnt->setModelMatrix(a);

	Entity* mEnt3 = mSceneManager->createEntity("ent3", "box.obj");
	SceneNode* node3 = mSceneManager->getRootSceneNode()->createChildSceneNode("node3", glm::vec3(0.5, 0.5, -1.0));
	node3->setScale(glm::vec3(0.25, 0.25, 0.25));
	node3->attachObject(mEnt3);

	node->lookAt(mCam->getPosition());

	mSceneManager->setCurrentCamera(mCam);
	Light* light1 = mSceneManager->createLight("light1");
	light1->setColor(glm::vec3(0.98, 1.0, 0.0));
	Light* light2 = mSceneManager->createLight("light2");
	light2->setPosition(glm::vec3(-0.8, 1.1, -0.9));
	light2->setColor(glm::vec3(1.0, 0.0, 0.49));
	Light* light3 = mSceneManager->createLight("light3");
	light3->setColor(glm::vec3(0.0, 0.98, 1.0));
	light3->setType(Light::SPOTLIGHT);
	//light3->setPosition(glm::vec3(0.0, 0.75, 0.0));
	light3->setDirection(glm::vec3(0.0, -1.0, 0.0));

	node->setScale(glm::vec3(0.2, 0.2, 0.2));;

	mEnt->attachMaterial("gold.mat");
	mEnt2->attachMaterial("pruebas");
	//mEnt2->getMaterial()->mBaseColorS.mBaseColorV = glm::vec3(1.0, 1.0, 1.0);


	if(InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		mCam->setControler(Camera::DEFAULT);
	}
	else
	{
		mCam->setControler(Camera::NOCONTROLER);
	}


	if(InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_G))
	{
		float a, b, c;
		std::cin >> a ;
		light3->setPosition(glm::vec3(0.0, a, 0.0));
	}

	if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_H))
	{
		light3->setPosition(glm::vec3(0.5, 0.0, -1.0));
	}

	
	for (int i = 1; i < mSceneManager->getNumOfLights() + 1; ++i)
	{
		Entity* ent = mSceneManager->createEntity("entLight" + std::to_string(i), "cone.obj");
		ent->setWireFrame(true);
		glm::mat4 a = glm::mat4(1.0);
		a = glm::translate(a, glm::vec3(0.0, -1.0, 0.0));
		ent->setModelMatrix(a);

		SceneNode* nod = mSceneManager->getRootSceneNode()->createChildSceneNode("entLight" + std::to_string(i));
		nod->attachObject(ent);

		nod->setPosition(mSceneManager->getLight("light" + std::to_string(i))->getPosition());
		glm::vec3 c = glm::normalize(glm::vec3(0.0, -1.0, 0.0));
		glm::vec3 b = glm::normalize(mSceneManager->getLight("light" + std::to_string(i))->getDirection());
		float cosa = glm::dot(c, b);
		glm::clamp(cosa, -1.0f, 1.0f);
		glm::vec3 axis = glm::cross(c, b);
		if (axis == glm::vec3(0.0))
		{
			axis = glm::vec3(1.0, 0.0, 0.0);
		}
		float angle = glm::degrees(glm::acos(cosa));
		nod->lookAt(mSceneManager->getLight("light" + std::to_string(i))->getDirection(), glm::vec3(0.0, -1.0, 0.0));
		nod->setScale(glm::vec3(mSceneManager->getLight("light" + std::to_string(i))->getRadius()));
	}
	
	//Bind framebuffer
	FrameBuffer* fbo = mSceneManager->getFrameBuffer("deferredFBO");
	fbo->bindForDrawing();

	//clear buffers
	glDepthMask(GL_TRUE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_DEPTH_TEST);

	//Process all the sceneNodes and renders all their attached objects
	mSceneManager->getRootSceneNode()->processRootSceneNode();

	//glDepthMask(GL_FALSE);
	//glDisable(GL_DEPTH_TEST);

	mSceneManager->processLights();

	fbo = mSceneManager->getFrameBuffer("lightFBO");
	fbo->bindForRendering();

	//swap the buffers
	mWindow->swapBuffers(true);
}