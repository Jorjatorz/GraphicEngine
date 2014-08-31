#include "UIManager.h"

#include "UIDisplayer.h"

UIManager::UIManager()
{
}


UIManager::~UIManager()
{
	tDisplayerMap::iterator it;
	for (it = mDisplayerMap.begin(); it != mDisplayerMap.end(); ++it)
	{
		delete it->second;
	}
	mDisplayerMap.clear();
}


UIDisplayer* UIManager::createDisplayer(std::string name)
{
	tDisplayerMap::iterator it = mDisplayerMap.find(name);
	if (it != mDisplayerMap.end())
	{
		return it->second;
	}

	UIDisplayer* mNewDisplayer = new UIDisplayer(name);

	mDisplayerMap.insert(std::pair<std::string, UIDisplayer*>(name, mNewDisplayer));

	return mNewDisplayer;
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
	else
	{
		return nullptr;
	}
}