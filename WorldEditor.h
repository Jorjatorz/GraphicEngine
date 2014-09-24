#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include "Definitions.h"

#include <vector>

class WorldEditor
{
public:
	WorldEditor(SceneManager* manager);
	~WorldEditor();

	//Executed every frame
	void processWorldEditor();

	//Select oobject
	void selectObject_RayCast(glm::vec3 cameraPos, glm::vec3 mouseDir_World);

private:
	SceneManager* mSceneManager;

	//Selected objects
	typedef std::vector<MovableObject*> tSelectedObjects;
	tSelectedObjects mSelectedObjects;
};

#endif