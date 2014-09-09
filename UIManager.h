#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "Definitions.h"

#include <iostream>
#include <map>
#include <string>

class UIManager
{
public:
	UIManager();
	~UIManager();

	UIDisplayer* createDisplayer(std::string name, SceneManager* manager);
	void deleteDisplayer(std::string name);
	UIDisplayer* getDisplayer(std::string name);
	void setCurrentDisplayer(UIDisplayer* disp); //Invoked by scenemanager
	UIDisplayer* getCurrentDisplayer()
	{
		return mCurrentUIDisplayer;
	}


	//Freetype - UIManager also init fonts library
	FT_Library mFreeTypeLibrary;
	FT_Face mFreeTypeFace;
private:
	typedef std::map<std::string, UIDisplayer*> tDisplayerMap;
	tDisplayerMap mDisplayerMap;
	UIDisplayer* mCurrentUIDisplayer;
};

#endif