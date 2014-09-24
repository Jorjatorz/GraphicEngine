#include "UIManager.h"

#include "UIDisplayer.h"
#include "UIDrawer.h"

UIManager::UIManager()
{
	//Initiate awwesonium
	mAwesomiumCore = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
	mAwesomiumCore->set_surface_factory(new UITextureSurfaceFactory());

	mCurrentDisplayer = NULL;
}


UIManager::~UIManager()
{
	tDisplayerMap::iterator it;
	for (it = mDisplayerMap.begin(); it != mDisplayerMap.end(); ++it)
	{
		delete it->second;
	}
	mDisplayerMap.clear();

	Awesomium::WebCore::Shutdown();
}


UIDisplayer* UIManager::createDisplayer(std::string name, SceneManager* manager)
{
	tDisplayerMap::iterator it = mDisplayerMap.find(name);
	if (it != mDisplayerMap.end())
	{
		return it->second;
	}

	UIDisplayer* newDisplayer = new UIDisplayer(name, manager, this);
	mDisplayerMap.insert(std::pair<std::string, UIDisplayer*>(name, newDisplayer));

	return newDisplayer;
}
void UIManager::deleteDisplayer(std::string name)
{
	tDisplayerMap::iterator it = mDisplayerMap.find(name);
	if (it != mDisplayerMap.end())
	{
		delete it->second;
		mDisplayerMap.erase(it);
	}

}
UIDisplayer* UIManager::getDisplayer(std::string name)
{
	tDisplayerMap::iterator it = mDisplayerMap.find(name);
	if (it != mDisplayerMap.end())
	{
		return it->second;
	}

	return NULL;
}

void UIManager::setCurrentDisplayer(UIDisplayer* displayer)
{
	mCurrentDisplayer = displayer;
}