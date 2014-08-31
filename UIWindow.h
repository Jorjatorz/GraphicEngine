#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "Definitions.h"
#include "UIObject.h"

#include <string>

class UIWindow: public UIObject
{
public:
	UIWindow(std::string name);
	~UIWindow();

private:

};

#endif