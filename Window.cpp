#include "Window.h"

#include "InputManager.h"

/*Window::Window(void)
{
}*/


Window::~Window(void)
{
	//delete the window
	SDL_GL_DeleteContext(mSDLContext);
}

Window::Window(std::string windowName, real width, real height, bool fullscreen)
{
	initWindow(windowName, width, height, fullscreen);
}

void Window::initWindow(std::string windowName, real width, real height, bool fullscreen)
{
	//Set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//create the window
	if(fullscreen)
		mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	else
		mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	mWindowWidth = width;
	mWindowHeight = height;
	mFullScreen = fullscreen;

	//create the context
	mSDLContext = SDL_GL_CreateContext(mSDLWindow);

	//Send window to the inputManager (so input relative  to the window is reported)
	InputManager::getSingletonPtr()->setSDLWindow(mSDLWindow);
}

void Window::swapBuffers(bool swap)
{
	//Swap buffers
	if(swap)
		SDL_GL_SwapWindow(mSDLWindow);
}

real Window::getWidth()
{
	return mWindowWidth;
}
real Window::getHeight()
{
	return mWindowHeight;
}
bool Window::isFullScreen()
{
	return mFullScreen;
}