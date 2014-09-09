#ifndef UILABEL_H
#define UILABEL_H

#include "Definitions.h"
#include "UIObject.h"

class UILabel : public UIObject
{
public:
	UILabel(std::string name, UIWindow* window, SceneManager* manager);
	~UILabel();

	void update();

private:
	UIWindow* mParentWindow;
};

#endif