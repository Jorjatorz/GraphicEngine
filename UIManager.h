#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "Definitions.h"

#include <string>
#include <map>

class UIManager
{
public:
	UIManager();
	~UIManager();

	void update()
	{
		mAwesomiumCore->Update();
	}

	UIDisplayer* createDisplayer(std::string name, SceneManager* manager);
	void deleteDisplayer(std::string name);
	UIDisplayer* getDisplayer(std::string name);
	void setCurrentDisplayer(UIDisplayer* displayer);
	UIDisplayer* getCurrentDisplayer()
	{
		return mCurrentDisplayer;
	}

	Awesomium::WebCore* getUICore()
	{
		return mAwesomiumCore;
	}

private:
	typedef std::map<std::string, UIDisplayer*> tDisplayerMap;
	tDisplayerMap mDisplayerMap;

	UIDisplayer* mCurrentDisplayer; //Same as the scenemanager current displayer

	//Awesomium
	Awesomium::WebCore* mAwesomiumCore;
};

#endif
