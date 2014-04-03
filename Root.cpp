#include "Root.h"


#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Root::Root(void)
{
	mRenderer = 0;
}


Root::~Root(void)
{
	delete mRenderer;
	delete mInput;
	delete mResourceManager;
}

void Root::initEngine()
{
	//creeate new resource manager
	mResourceManager = new ResourceManager();

	//create a new renderer
	mRenderer = new Renderer();
	mRenderer->createRenderer("Pruebas", 1280, 720, false);

	//create new input manager
	mInput = new InputManager();

	bool running = true;
	while(running && !mInput->isKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		//Get input
		mInput->getFrameInput(running);

		mRenderer->renderFrame();
		
		SDL_Delay(16);
	}

}

SceneManager* Root::createSceneManager(std::string name)
{
	//find if the sceneManager already exists
	tSceneManagerMap::iterator sceneMIterator;
	for(sceneMIterator = mSceneManagerMap.begin(); sceneMIterator != mSceneManagerMap.end(); ++sceneMIterator)
	{
		//if it exist return it
		if(sceneMIterator->first == name)
		{
			return sceneMIterator->second;
		}
	}
	//else
	SceneManager* mSceneManager = new SceneManager();

	mSceneManagerMap.insert(std::pair<std::string, SceneManager*>(name, mSceneManager));

	return mSceneManager;
}
void Root::deleteSceneManager(std::string name)
{
	//find if the sceneManager already exists
	tSceneManagerMap::iterator sceneMIterator;
	for(sceneMIterator = mSceneManagerMap.begin(); sceneMIterator != mSceneManagerMap.end(); ++sceneMIterator)
	{
		//if it exist delete it
		if(sceneMIterator->first == name)
		{
			//delete the sceneManager and remove from the map
			delete sceneMIterator->second;
			mSceneManagerMap.erase(sceneMIterator);
			//iterator now is invalid so we break from the loop or execution error
			break;
		}
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