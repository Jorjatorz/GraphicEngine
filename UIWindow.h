#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "Definitions.h"
#include "UICallbackListener.h"

#include <string>

//Doesnt use ResourceManager, its a Use - Delete class resoruces (included textures).
class UIWindow
{
public:
	UIWindow(std::string name, real Width, real Height, std::string uiFilePath, SceneManager* manager);
	~UIWindow();

	void render(Shader* uiShad);

	//Called by the UI, swaps the resize mode ot on-off
	void resize();
	void checkForResize(); //Executed every frame, if true it resize the window
	//Called by the UI, moves the window
	void moveWindow();
	void checkForMove(); //Executed every frame, if true it moves the window


	bool rayTestToQuad(glm::vec2 pointPos);

	void setMouseMove(glm::vec2 mousePos);
	void setMouseButtonDown();
	void setMouseButtonUp();

	//add a new function callback - Templates must be declare un .h
	template<typename T>
	void addCallbackReciver(std::string mJSCallbackFunction, void (T::*functionToExecutePtr)(), T* obj)
	{
		mCallbackListener.addFunctionCallback(mJSCallbackFunction, functionToExecutePtr, obj);
	}
	UICallbackListener* getCallBackListener()
	{
		return &mCallbackListener;
	}

	//Awesomium view
	Awesomium::WebView* mAwesomiumView;
private:
	std::string mName;
	real mWidth, mHeight;
	glm::vec2 mPosition;
	glm::vec2 mSize; //From '1 to 1
	SceneManager* mCurrentSceneManager;

	//Resize & move mode
	bool bResize;
	bool bMove;
	glm::vec2 mLastMousePosition;

	//Opengl
	GLuint VAO_, VBO_;
	void createQuad();

	//Awesomium
	Awesomium::WebCore* mAwesomiumCore;

	//Callback listener
	UICallbackListener mCallbackListener;
};

#endif