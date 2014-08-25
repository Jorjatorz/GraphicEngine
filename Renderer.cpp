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
#include "ResourceManager.h"
#include "RigidBody.h"
#include "PhysicsManager.h"
#include "RayCast.h"
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


int gh = 0;
void Renderer::renderFrame(real deltaTime)
{
	//PRuebas
	mSceneManager->mDeltaTime = deltaTime;


	Camera* mCam = mSceneManager->createCamera("camera1");
	mSceneManager->setCurrentCamera(mCam);

	Entity* mEnt = mSceneManager->createEntity("cube", "guard.obj");
	Shader* mShader = mSceneManager->createShader("basic", "basic");
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("nod", glm::vec3(0.0, 0.0, -1.0));
	mSceneManager->setPerspectiveMatrix(60, mWindow->getWidth(), mWindow->getHeight(), 0.01);

	//mCam->setControler(Camera::DEFAULT);
	mSceneManager->setCurrentShader(mShader);

	Entity* mEnt2 = mSceneManager->createEntity("ent2", "plane.obj");
	SceneNode* node2 = mSceneManager->getRootSceneNode()->createChildSceneNode("nod2");
	//node2->rotate(glm::vec3(0.0, 1.0, 0.0), 1.0);
	node2->setPosition(glm::vec3(-1.0, -0.25, 0.0));
	node2->setScale(glm::vec3(2.0, 2.0, 2.0));
	node2->attachObject(mEnt2);
	//mEnt2->makeRigidBody();
	//mEnt2->getRigidBody()->setCollisionShape_Box(mEnt2->getMesh()->getAABBsize() * 2.0f);
	//mEnt2->getRigidBody()->setUpRigidBody(0.0, node2, mEnt2);
	node->attachObject(mEnt);
	mEnt->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -0.76, 0.0)));
	//Ent->makeRigidBody();
	//mEnt->getRigidBody()->setCollisionShape_Box((mEnt->getMesh()->getAABBsize() / 5.0f));
	//mEnt->getRigidBody()->setUpRigidBody(1.0, node, mEnt);

	Entity* mEnt3 = mSceneManager->createEntity("ent3", "box.obj");
	//mEnt3->makeRigidBody();
	//mEnt3->getRigidBody()->setCollisionShape_Box(glm::vec3(0.5, 0.5, 0.5));
	SceneNode* node3 = mSceneManager->getRootSceneNode()->createChildSceneNode("node3", glm::vec3(0.5, 0.5, -1.0));
	node3->setScale(glm::vec3(0.25, 0.25, 0.25));
	node3->attachObject(mEnt3);
	//mEnt3->getRigidBody()->setUpRigidBody(1.0, node3, mEnt3);


	node->setScale(glm::vec3(0.2, 0.2, 0.2));;

	mEnt->attachMaterial("gold.mat");
	mEnt2->attachMaterial("pruebas");

	Light* light1 = mSceneManager->createLight("light1");
	light1->setType(Light::DIRECTIONALLIGHT);
	light1->setDirection(glm::vec3(0.0, -0.5, -1.0));
	light1->setColor(glm::vec3(1.0, 1.0, 1.0));
	Light* light2 = mSceneManager->createLight("light2");
	light2->setPosition(glm::vec3(0.0, 0.1, -2.0));
	light2->setColor(glm::vec3(0.98, 1.0, 0.0));
	Light* light3 = mSceneManager->createLight("light3");
	light3->setColor(glm::vec3(0.0, 0.98, 1.0));
	light3->setType(Light::SPOTLIGHT);
	//light3->setPosition(glm::vec3(0.0, 0.75, 0.0));
	light3->setDirection(glm::vec3(0.0, -1.0, 0.0));

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
		Entity* ent = mSceneManager->createEntity("as" + std::to_string(gh), "guard.obj");
		SceneNode* nodes = mSceneManager->getRootSceneNode()->createChildSceneNode("nodes" + std::to_string(gh), mCam->getPosition());
		nodes->setScale(glm::vec3(0.05, 0.05, 0.05));
		nodes->attachObject(ent);
		ent->getRigidBody()->setMass(1.0);
		ent->getRigidBody()->setLinearVelocity(mCam->getOrientation() * 2.0f);

		gh++;

		std::cout << "gh: " << gh << std::endl;
	}

	if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_H))
	{
		mEnt3->getRigidBody()->setMass(1.0);
	}

	/*
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
		glm::vec3 e = glm::vec3(glm::vec3(0.0, 1.0, 0.0) * (mSceneManager->getLight("light" + std::to_string(i))->getAttenuationRadius() * 0.5f));
		glm::vec3 f =  glm::vec3(1.0, 0.0, 1.0) * (mSceneManager->getLight("light" + std::to_string(i))->getAttenuationRadius() * glm::tan(glm::radians(mSceneManager->getLight("light" + std::to_string(i))->getOuterAngle())));
		nod->setScale(e + f);
	}*/
	
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

	mSceneManager->bindShader(mSceneManager->createShader("old", "old"));
	mSceneManager->getCurrentShader()->UniformMatrix("PV", mSceneManager->getProjectionMatrix() * mSceneManager->getViewMatrix());
	PhysicsManager::getSingletonPtr()->draw();

	//Process the lights
	mSceneManager->processLights();

	//Render the combined buffer
	fbo = mSceneManager->getFrameBuffer("lightFBO");
	fbo->bindForRendering();


	//swap the buffers
	mWindow->swapBuffers(true);
}