#ifndef ROOT_H
#define ROOT_H

#include "Definitions.h"

#include "Singleton.h"

#include <map>

class Root : public Singleton<Root>
{
public:
	Root(void);
	~Root(void);

	void initEngine();


	//singletons
	InputManager* mInput;
	ResourceManager* mResourceManager;
	PhysicsManager* mPhysicsManager;

	//classes
	Renderer* mRenderer; //will be a list

	//SceneManager creator and destructors
	SceneManager* createSceneManager(std::string name, Renderer* callingRenderer);
	void deleteSceneManager(std::string name);
	SceneManager* getCurrentSceneManager()
	{
		return mCurrentSceneManager;
	}

	//Timer
	Timer* createTimer(std::string name);
	void deleteTimer(std::string name);
	Timer* getTimer(std::string name);

private:
	//SceneManagerMap
	typedef std::map<std::string, SceneManager*> tSceneManagerMap;
	tSceneManagerMap mSceneManagerMap;
	SceneManager* mCurrentSceneManager;
	void deleteAllSceneManagers();

	//Timers maps
	typedef std::map<std::string, Timer*> tTimerMap;
	tTimerMap mTimerMap;
	void deleteAllTimers();
};

#endif