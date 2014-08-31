#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "Definitions.h"

#include <map>
#include <string>

class UIManager
{
public:
	UIManager();
	~UIManager();

	UIDisplayer* createDisplayer(std::string name);
	void deleteDisplayer(std::string name);
	UIDisplayer* getDisplayer(std::string name);

private:
	typedef std::map<std::string, UIDisplayer*> tDisplayerMap;
	tDisplayerMap mDisplayerMap;
};

#endif