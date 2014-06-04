#include "Root.h"


#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Root::Root(void)
{
	mRenderer = 0;
	mCurrentSceneManager = NULL;
}


Root::~Root(void)
{
	deleteAllTimers();
	deleteAllSceneManagers();
	delete mRenderer;
	delete mInput;
	delete mResourceManager;
}
#include "Timer.h"
void Root::initEngine()
{
	//Create singletons---------------
	//creeate new resource manager
	mResourceManager = new ResourceManager();
	//create new input manager
	mInput = new InputManager();
	//--------------------------------
	//create a new renderer
	mRenderer = new Renderer();
	mRenderer->createRenderer("Pruebas", 1280, 720, false);

	//MAIN LOOP
	bool running = true;

	//Pruebas
	Timer* mTimer = createTimer("fpsTimer");
	int frames = 0;
	int miliSeconds = 0;
	int framesPerSecond = 1000/60;
	mTimer->getTicks();
	//
	while(running && !mInput->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		mTimer->getTicks();
		//Get input
		mInput->getFrameInput(running);
		//Render a frame
		mRenderer->renderFrame(mTimer->getDeltaTicks() * 0.001);

		//compute delta
		miliSeconds += mTimer->getDeltaTicks();

		//if we are too fast
		if(mTimer->mDeltaTime < framesPerSecond)
		{
			SDL_Delay(framesPerSecond - mTimer->mDeltaTime);
			miliSeconds += framesPerSecond - mTimer->mDeltaTime;
		}

		frames++;
		if(miliSeconds > 1000)
		{
			std::cout << frames << std::endl;
			miliSeconds = 0;
			frames = 0;
		}
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