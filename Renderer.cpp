#include "Renderer.h"

#include "Root.h"
#include "SceneManager.h"
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
	glEnable(GL_DEPTH_TEST);

	//GL STATES AND ENABLES
}

//delete this
#include "Shader.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
//

real caca = 0;
void Renderer::renderFrame(real deltaTime)
{
	//PRuebas
	mSceneManager->mDeltaTime = deltaTime;

	Entity* mEnt = mSceneManager->createEntity("cube", "guard.obj");
	//Entity* mEnt2 = mSceneManager->createEntity("cube2", "NULL");
	Shader* mShader = mSceneManager->createShader("basic", "basic");
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("nod", glm::vec3(0.0, 0.0, -1.0));
	mSceneManager->setPerspectiveMatrix(60,  mWindow->getWidth(), mWindow->getHeight(), 0.1);
	Camera* mCam = mSceneManager->createCamera("camera1");
	mCam->setControler(Camera::DEFAULT);
	mSceneManager->setCurrentShader(mShader);

	Entity* mEnt2 = mSceneManager->createEntity("ent2", "spider.obj");
	SceneNode* node2 = node->createChildSceneNode("nod2", glm::vec3(2.0, 0.0, 0.0));
	node2->setOrientation(glm::vec3(0.0, -90.0, 0.0));
	node2->attachObject(mEnt2);
	node->attachObject(mEnt);

	mSceneManager->setCurrentCamera(mCam);

	node->setScale(glm::vec3(0.2, 0.2, 0.2));
	node2->setScale(glm::vec3(0.01, 0.01, 0.01));



	if(InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_C))
	{
		mCam->setControler(Camera::NOCONTROLER);
		mCam->lookAt(node->getPosition());
	}

	//clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//NO pruebas

	//Process all the sceneNodes and renders all their attached objects
	mSceneManager->getRootSceneNode()->processRootSceneNode();

	//swap the buffers
	mWindow->swapBuffers(true);
}