#include "WorldEditor.h"

#include "RayCast.h"
#include "MovableObject.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIDisplayer.h"
#include "Entity.h"
#include "UIWindow.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Material.h"
#include "RigidBody.h"

WorldEditor::WorldEditor(SceneManager* manager)
{
	mSceneManager = manager;

	mEditorDisplayer = mSceneManager->createDisplayer("worldEditorDisplayer");
	mSceneManager->setCurrentDisplayer("worldEditorDisplayer");
	mLastMousePos = glm::vec2(-99999.9);
	mDraggingAxis_X = mDraggingAxis_Y = mDraggingAxis_Z = false;
	mTransformationMode = SCALE;
}


WorldEditor::~WorldEditor()
{
}

void WorldEditor::processWorldEditor()
{
	for (int i = 0; i < mSelectedObjects.size(); ++i)
	{
		mSelectedObjects.at(i)->showAABB(true);

		switch (mSelectedObjects.at(i)->getType())
		{
		case MovableObject::Entity:
		{
										entityEditor(static_cast<Entity*>(mSelectedObjects.at(i)));
										break;
		}
		case MovableObject::Light:
		{
										break;
		}
		case MovableObject::Camera:
		{
										break;
		}
		}
	}

	if (!mSelectedObjects.empty())
	{
		//Move the axis
		checkForAxisDrag();
		//DrawAxis
		drawAxis(static_cast<Entity*>(mSelectedObjects.at(0)));
	}
}

void WorldEditor::selectObject_RayCast(glm::vec3 cameraPos, glm::vec3 mouseDir_World)
{
	RayCast mRay;
	mRay.executeRaySelectionCast(cameraPos, mouseDir_World * 1000.0f);

	if (mRay.hasHits())
	{
		if (mRay.getHitObject()->getName() == "Xaxis_WorldEditor")
		{
			mDraggingAxis_X = true;
		}
		else if (mRay.getHitObject()->getName() == "Yaxis_WorldEditor")
		{
			mDraggingAxis_Y = true;
		}
		else if (mRay.getHitObject()->getName() == "Zaxis_WorldEditor")
		{
			mDraggingAxis_Z = true;
		}
		else
		{
			if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_LCTRL))
			{
				mSelectedObjects.push_back(mRay.getHitObject());
			}
			else //Just one selection
			{
				//Remove AABB of old selections
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->showAABB(false);
				}

				mSelectedObjects.clear();

				mSelectedObjects.push_back(mRay.getHitObject());

				//Save physics type and make it kinetic so we can move it
				physicsSavedType = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getType();
				physicsSavedMass = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getMass();
				static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->setMass(0.0, false);
			}
		}
	}
	else
	{
		//Remove AABB
		for (int i = 0; i < mSelectedObjects.size(); ++i)
		{
			mSelectedObjects.at(i)->showAABB(false);
		}

		//Reset to the old physics props
		static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->setMass(physicsSavedMass, physicsSavedType);

		mSelectedObjects.clear();
	}
}

void WorldEditor::entityEditor(Entity* ent)
{
	//Handle the input for world trans
	if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_W))
	{
		mTransformationMode = TRANSLATION;
	}
	else if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_E))
	{
		mTransformationMode = SCALE;
	}


	//Create the UI Window
	mEditorDisplayer->createUIWindow("entityWindow", 400, 400, "entitySelected.html")->setPosition_NDC(glm::vec2(-0.65, 0.45));

	glm::vec3 position, orientation, scale;
	position = ent->getPosition();
	orientation = ent->getOrientation_Euler();
	scale = ent->getScale();
	//Name
	mEditorDisplayer->setPropertyToWindow("entityWindow", "entName", "innerHTML", ent->getName());
	//Position
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textP1", "value", position.x);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textP2", "value", position.y);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textP3", "value", position.z);
	//Orientation
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textO1", "value", orientation.x);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textO2", "value", orientation.y);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textO3", "value", orientation.z);
	//Scale
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textS1", "value", scale.x);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textS2", "value", scale.y);
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textS3", "value", scale.z);
	//Physics
	mEditorDisplayer->setPropertyToWindow("entityWindow", "textMass", "value", ent->getMass());
}

void WorldEditor::drawAxis(Entity* firstEntity)
{
	SceneNode* Xnod = mSceneManager->getRootSceneNode()->createChildSceneNode("Xnod");
	SceneNode* Ynod = mSceneManager->getRootSceneNode()->createChildSceneNode("Ynod");
	SceneNode* Znod = mSceneManager->getRootSceneNode()->createChildSceneNode("Znod");
	//axis
	Entity* Xent = mSceneManager->createEntity("Xaxis_WorldEditor");
	Entity* Yent = mSceneManager->createEntity("Yaxis_WorldEditor");
	Entity* Zent = mSceneManager->createEntity("Zaxis_WorldEditor");

	if (mTransformationMode == tTransformationModeEnum::TRANSLATION)
	{
		Xent->attachMesh("axis_translation.obj");
		Yent->attachMesh("axis_translation.obj");
		Zent->attachMesh("axis_translation.obj");
	}
	else
	{
		Xent->attachMesh("axis_scale.obj");
		Yent->attachMesh("axis_scale.obj");
		Zent->attachMesh("axis_scale.obj");
	}



	//Set nodes positions
	glm::vec3 entPos = firstEntity->getPosition();
	Xnod->setPosition(entPos + glm::vec3(0.5, 0.0, 0.0));
	Xnod->rotate(glm::vec3(0.0, 0.0, 1.0), -90.0);
	Ynod->setPosition(entPos + glm::vec3(0.0, 0.5, 0.0));
	Znod->setPosition(entPos + glm::vec3(0.0, 0.0, 0.5));
	Znod->rotate(glm::vec3(1.0, 0.0, 0.0), 90.0);

	Xnod->attachObject(Xent);
	Ynod->attachObject(Yent);
	Znod->attachObject(Zent);


	//Set materials and kinematic atributtes
	//Translation
	Xent->setMass(0.0, false);
	Xent->attachMaterial("XaxisMat");
	Xent->getMaterial()->setBaseColorV(glm::vec3(1.0, 0.0, 0.0));
	Yent->setMass(0.0, false);
	Yent->attachMaterial("YaxisMat");
	Yent->getMaterial()->setBaseColorV(glm::vec3(0.0, 0.0, 1.0));
	Zent->setMass(0.0, false);
	Zent->attachMaterial("ZaxisMat");
	Zent->getMaterial()->setBaseColorV(glm::vec3(0.0, 1.0, 0.0));
	
}

void WorldEditor::checkForAxisDrag()
{
	if (InputManager::getSingletonPtr()->isMouseButtonUp(SDL_BUTTON_LEFT))
	{
		mDraggingAxis_X = mDraggingAxis_Y = mDraggingAxis_Z = false;
		mLastMousePos = glm::vec2(-99999.9);
	}
	//Check if alt is pressed for uniform scaling (uniform scaling = scale the same amount in all the axis at the same time)
	bool uniformScaling = false;
	if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_LALT))
	{
		uniformScaling = true;
	}

	if (mDraggingAxis_X)
	{
		if (mLastMousePos == glm::vec2(-99999.9))
		{
			InputManager::getSingletonPtr()->getMousePosition(mLastMousePos);
		}

		glm::vec2 mouse;
		InputManager::getSingletonPtr()->getMousePosition(mouse);
		if (mTransformationMode == tTransformationModeEnum::TRANSLATION)
		{
			mSelectedObjects.at(0)->mParentSceneNode->translate(glm::vec3((mouse.x - mLastMousePos.x) / 10.0, 0.0, 0.0)); //The selected object is the axis so translate it
		}
		else
		{
			if (uniformScaling)
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
			}
			else
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0, 0.0, 0.0)); //The selected object is the axis so scale it
			}
		}
		mLastMousePos = mouse;
	}
	else if (mDraggingAxis_Y)
	{
		if (mLastMousePos == glm::vec2(-99999.9))
		{
			InputManager::getSingletonPtr()->getMousePosition(mLastMousePos);
		}

		glm::vec2 mouse;
		InputManager::getSingletonPtr()->getMousePosition(mouse);
		if (mTransformationMode == tTransformationModeEnum::TRANSLATION)
		{
			mSelectedObjects.at(0)->mParentSceneNode->translate(glm::vec3(0.0, -(mouse.y - mLastMousePos.y) / 10.0, 0.0)); //The selected object is the axis so translate it
		}
		else
		{
			if (uniformScaling)
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
			}
			else
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3(0.0, -(mouse.y - mLastMousePos.y) / 100.0, 0.0)); //The selected object is the axis so scale it
			}
		}
		mLastMousePos = mouse;
	}
	else if (mDraggingAxis_Z)
	{
		if (mLastMousePos == glm::vec2(-99999.9))
		{
			InputManager::getSingletonPtr()->getMousePosition(mLastMousePos);
		}

		glm::vec2 mouse;
		InputManager::getSingletonPtr()->getMousePosition(mouse);
		if (mTransformationMode == tTransformationModeEnum::TRANSLATION)
		{
			mSelectedObjects.at(0)->mParentSceneNode->translate(glm::vec3(0.0, 0.0, -(mouse.x - mLastMousePos.x) / 10.0)); //The selected object is the axis so translate it
		}
		else
		{
			if (uniformScaling)
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
			}
			else
			{
				mSelectedObjects.at(0)->mParentSceneNode->scale(glm::vec3(0.0, 0.0, -(mouse.x - mLastMousePos.x) / 100.0)); //The selected object is the axis so scale it
			}
		}
		mLastMousePos = mouse;
	}

	static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->setTransforms(static_cast<Entity*>(mSelectedObjects.at(0))->mParentSceneNode);
}