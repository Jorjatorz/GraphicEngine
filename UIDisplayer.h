#ifndef UIDISPLAYER_H
#define UIDISPLAYER_H

#include "Definitions.h"

#include <map>
#include <string>

class UIDisplayer
{
public:
	UIDisplayer(std::string name);
	~UIDisplayer();

	void drawDisplayer(Shader* UIShader); //Draw all windows and its elements

	UIWindow* createWindow(std::string name);
	void deleteWindow(std::string name);
	UIWindow* getWindow(std::string name);

private:
	std::string mName;

	typedef std::map<std::string, UIWindow*> tWindowsMap;
	tWindowsMap mWindowsMap;
};

#endif