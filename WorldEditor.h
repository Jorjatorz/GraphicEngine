#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include "Definitions.h"

#include "RigidBody.h"

#include <vector>

class WorldEditor
{
public:
	WorldEditor(SceneManager* manager);
	~WorldEditor();

	//Executed every frame
	void processWorldEditor();

	void drawAxis(MovableObject* firstObject); //First object of the selection map
	void entityEditor(Entity* ent);
	void lightEditor(Light* light);

	//Select oobject
	//Called by the inputmanager (through scenemanager)
	void selectObject_RayCast(glm::vec3 cameraPos, glm::vec3 mouseDir_World);

private:
	SceneManager* mSceneManager;
	UIDisplayer* mEditorDisplayer;

	typedef enum
	{
		TRANSLATION,
		ROTATION,
		SCALE
	} tTransformationModeEnum;
	tTransformationModeEnum mTransformationMode;

	//Selected objects
	typedef std::vector<MovableObject*> tSelectedObjects;
	tSelectedObjects mSelectedObjects;
	MovableObject* mPreviousSelectedObject; //Save the previous selected object so we check if it has changed, if yes, render again the information to the ui

		//Save the physics props, so we can move all object in
		typedef struct
		{
			RigidBody::tRigidBodyType mRigidBodyType;
			real mRigidBodyMass;
		} tPhysicsPropsSaverStruct;
		typedef std::vector<tPhysicsPropsSaverStruct> tPhysicsSavedVector;
		tPhysicsSavedVector mPhysicsSavedProperties;

	//Axis staff
	glm::vec2 mLastMousePos;
	bool mDraggingAxis_X, mDraggingAxis_Y, mDraggingAxis_Z;
	void checkForAxisDrag(); //Checks if an axis is being dragged and move the entity
};

#endif