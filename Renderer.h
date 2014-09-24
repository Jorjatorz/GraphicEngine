#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <string>

#include "Definitions.h"



class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	void createRenderer(std::string windowName, int width, int height, bool fullscreen);

	//render a frame
	void renderFrame(real deltaTime);

	void a();

	//swap front and back buffer
	//void swapBuffers();

	Window* getCurrentWindow()
	{
		return mWindow;
	}


private:
	//prototypes
	//call initial functions to initialize opengl
	void initOpenGL();

	//Classes pointers
	Window* mWindow;

	SceneManager* mSceneManager;


};

#endif