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
	mSceneManager = mRoot->createSceneManager("pruebas");
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
void Renderer::renderFrame()
{
	//PRuebas
	Entity* mEnt = mSceneManager->createEntity("cube", "NULL");
	Entity* mEnt2 = mSceneManager->createEntity("cube2", "NULL");
	Shader* mShader = mSceneManager->createShader("basic", "basic");
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("nod", glm::vec3(0.0, 0.0, -3.0));
	mSceneManager->setPerspectiveMatrix(60,  mWindow->getWidth(), mWindow->getHeight(), 0.1);
	Camera* mCam = mSceneManager->createCamera("camera1");
	Camera* mCam2 = mSceneManager->createCamera("camera2");
	mCam->lookAt(glm::vec3(-1.0, 1.0, -5.0));
	mSceneManager->setCurrentShader(mShader);


	if(Root::getSingletonPtr()->mInput->isKeyDown(SDL_SCANCODE_W))
	{
		mSceneManager->setCurrentCamera(mCam);
	}
	else
	{
		mSceneManager->setCurrentCamera(mCam2);
	}

	node->setScale(glm::vec3(0.1, 0.1, 0.1));
	node->translate(glm::vec3(0.001, 0.001, 0.001));

	//clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mEnt->setCubeMesh();
	mEnt2->setCubeMesh();
	//mEnt->render(glm::mat4(1.0));
	node->attachObject(mEnt);	
	//NO pruebas
	//Process all the sceneNodes and renders all their attached objects
	mSceneManager->getRootSceneNode()->processRootSceneNode();
	//swap the buffers
	mWindow->swapBuffers(true);
}