#include "UILabel.h"

#include "UIWindow.h"

UILabel::UILabel(std::string name, UIWindow* window, SceneManager* manager)
{
	mName = name;
	mParentWindow = window;
	mCurrentSceneManager = manager;

	mSize = glm::vec2(0.2);
	mTextPosition = -mSize / 4.0f;
	//mTextSize = glm::vec2(0.046 / 10, 0.011);

	mText = "Label";

	//mTextPosition = -mSize / 2.0f;
}


UILabel::~UILabel()
{
}

void UILabel::update()
{
	mParentPosition = mParentWindow->getPosition();
	mParentSize = mParentWindow->getSize();

	mTextSize = mSize * mParentSize / 50.0f;

	mTextPosition = -mSize * mParentSize / 50.0f;
}