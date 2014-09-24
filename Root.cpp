#include "Root.h"


#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Timer.h"
#include "UIManager.h"

Root::Root(void)
{
	mRenderer = NULL;
	mUIManager = NULL;
	mCurrentSceneManager = NULL;
}


Root::~Root(void)
{
	deleteAllTimers();
	deleteAllSceneManagers();
	delete mPhysicsManager;
	delete mUIManager;
	delete mRenderer;
	delete mInput;
	delete mResourceManager;
}

void Root::initEngine()
{
	//Create singletons---------------
	//creeate new resource manager
	mResourceManager = new ResourceManager();
	//create new input manager
	mInput = new InputManager();
	//Create new physics manager
	mPhysicsManager = new PhysicsManager();
	//--------------------------------

	//Create physics world
	mPhysicsManager->initPhysicsWorld();
	//create new ui
	mUIManager = new UIManager();
	//create a new renderer
	mRenderer = new Renderer();
	mRenderer->createRenderer("Pruebas", 1280, 720, false);

	//MAIN LOOP
	bool running = true;

	//Pruebas
	Timer* mTimer = createTimer("fpsTimer");
	Timer* timer2 = createTimer("renderTimer");
	Timer* timer3 = createTimer("physicsTimer");
	int frames = 0;
	int miliSeconds = 0;
	int framesPerSecond = 1000/60;
	int lastTime = 0;
	mTimer->start();
	//
	while(running && !mInput->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		mTimer->start();

		//Get input
		mInput->getFrameInput(running);

		timer3->start();
		mPhysicsManager->processPhysicsWorld((mTimer->mStartTime - lastTime) * 0.001);
		timer3->stop();

		//Render a frame
		timer2->start();
		mRenderer->renderFrame((mTimer->mStartTime - lastTime) * 0.001);
		timer2->stop();

		if (mTimer->getMiliSeconds() < framesPerSecond)
		{
			SDL_Delay(framesPerSecond - mTimer->getMiliSeconds());
		}

		miliSeconds += mTimer->getMiliSeconds();

		frames++;
		if (miliSeconds > 1000)
		{
			std::cout << "Renderer: " << (float)timer2->mToltalTime / frames << std::endl;
			std::cout << "Physics: " << (real)timer3->mToltalTime / frames << std::endl;
			std::cout << "Total: " << (float)mTimer->mToltalTime / frames << std::endl;
			mTimer->mToltalTime = 0;
			timer2->reset();
			timer3->reset();
			std::cout << frames << std::endl;
			frames = 0;
			miliSeconds = 0;
		}
		
		lastTime = mTimer->mStartTime; //Get the duration of the previous frame
		mTimer->stop();
	}

}

SceneManager* Root::createSceneManager(std::string name, Renderer* callingRenderer)
{
	//find if the sceneManager already exists
	tSceneManagerMap::iterator sceneMIterator = mSceneManagerMap.find(name);

	//if it exist return it
	if(sceneMIterator != mSceneManagerMap.end())
	{
		return sceneMIterator->second;
	}

	//else
	SceneManager* mSceneManager = new SceneManager(callingRenderer);

	mSceneManagerMap.insert(std::pair<std::string, SceneManager*>(name, mSceneManager));

	mCurrentSceneManager = mSceneManager;

	return mSceneManager;
}
void Root::deleteSceneManager(std::string name)
{
	//find if the sceneManager already exists
	//find if the sceneManager already exists
	tSceneManagerMap::iterator sceneMIterator = mSceneManagerMap.find(name);

	//if it exist delete it
	if (sceneMIterator != mSceneManagerMap.end())
	{
			//delete the sceneManager and remove from the map
			delete sceneMIterator->second;
			mSceneManagerMap.erase(sceneMIterator);
	}

}
void Root::deleteAllSceneManagers()
{
	//iterate through all the map and delete all components
	tSceneManagerMap::iterator sceneMIterator;
	for(sceneMIterator = mSceneManagerMap.begin(); sceneMIterator != mSceneManagerMap.end(); ++sceneMIterator)
	{			//delete the sceneManager
			delete sceneMIterator->second;
	}

	//clear the map
	mSceneManagerMap.clear();
}

Timer* Root::createTimer(std::string name)
{
	tTimerMap::iterator it = mTimerMap.find(name);

	if (it != mTimerMap.end())
	{
		return it->second;
	}
	else
	{
		Timer* newTimer = new Timer(name);

		mTimerMap.insert(std::pair<std::string, Timer*>(name, newTimer));

		return newTimer;
	}
}

void Root::deleteTimer(std::string name)
{
	tTimerMap::iterator it = mTimerMap.find(name);

	if (it != mTimerMap.end())
	{
		delete it->second;
	}
}

Timer* Root::getTimer(std::string name)
{
	tTimerMap::iterator it = mTimerMap.find(name);

	if (it != mTimerMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void Root::deleteAllTimers()
{
	tTimerMap::iterator it;

	for (it = mTimerMap.begin(); it != mTimerMap.end(); ++it)
	{
		delete it->second;
	}

	mTimerMap.clear();
}