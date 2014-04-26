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

	//classes
	Renderer* mRenderer; //will be a list

	//SceneManager creator and destructors
	SceneManager* createSceneManager(std::string name, Renderer* callingRenderer);
	void deleteSceneManager(std::string name);

private:
	//SceneManagerMap
	typedef std::map<std::string, SceneManager*> tSceneManagerMap;
	tSceneManagerMap mSceneManagerMap;
	SceneManager* mCurrentSceneManager;
	void deleteAllSceneManagers();
};

#endif