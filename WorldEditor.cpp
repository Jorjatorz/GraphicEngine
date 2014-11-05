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
	mTransformationMode = TRANSLATION;
	mPreviousSelectedObject = NULL;
}


WorldEditor::~WorldEditor()
{
	mPhysicsSavedProperties.clear();
}

void WorldEditor::processWorldEditor()
{
	for (int i = 0; i < mSelectedObjects.size(); ++i)
	{
		mSelectedObjects.at(i)->showAABB(true);

		switch (mSelectedObjects.at(i)->getType())
		{
		case MovableObject::ENTITY:
		{
										entityEditor(static_cast<Entity*>(mSelectedObjects.at(i)));
										break;
		}
		case MovableObject::LIGHT:
		{
										break;
		}
		case MovableObject::CAMERA:
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
	else
	{
		//Dont draw DrawAxis
		drawAxis(NULL);
		//Dont draw the window
		if (mEditorDisplayer->getWindow("entityWindow_WorldEditor") != NULL)
		{
			mEditorDisplayer->getWindow("entityWindow_WorldEditor")->setVisible(false);
		}
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
				//Check if we have already selected that object
				tSelectedObjects::iterator it = std::find(mSelectedObjects.begin(), mSelectedObjects.end(), mRay.getHitObject());
				if (it == mSelectedObjects.end())
				{
					if (mRay.getHitObject()->getType() == MovableObject::ENTITY)
					{
						Entity* ent = static_cast<Entity*>(mRay.getHitObject());
						tPhysicsPropsSaverStruct newPhysicsProps;
						newPhysicsProps.mRigidBodyType = ent->getRigidBody()->getType();
						newPhysicsProps.mRigidBodyMass = ent->getRigidBody()->getMass();
						ent->setMass(0.0, false);

						mPhysicsSavedProperties.push_back(newPhysicsProps);
					}

					mSelectedObjects.push_back(mRay.getHitObject());
				}
			}
			else //Just one selection
			{
				//Remove AABB of old selections
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->showAABB(false);
				}

				mSelectedObjects.clear();
				mPreviousSelectedObject = NULL;

				mSelectedObjects.push_back(mRay.getHitObject());

				mPhysicsSavedProperties.clear();
				if (mSelectedObjects.at(0)->getType() == MovableObject::ENTITY)
				{
					tPhysicsPropsSaverStruct newPhysicsProps;
					newPhysicsProps.mRigidBodyType = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getType();
					newPhysicsProps.mRigidBodyMass = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getMass();
					//Set as kinetic object so we are able to move it
					static_cast<Entity*>(mSelectedObjects.at(0))->setMass(0.0, false);

					mPhysicsSavedProperties.push_back(newPhysicsProps);
				}

				/*if (mSelectedObjects.at(0)->getType() == MovableObject::ENTITY)
				{
					//Save physics type and make it kinetic so we can move it
					mPhysicsSavedType = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getType();
					mPhysicsSavedMass = static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->getMass();
					static_cast<Entity*>(mSelectedObjects.at(0))->setMass(0.0, false);
				}*/
			}

		}
	}
	else
	{
		//If control is not preset reset selections (this prevents the reset of all selections if the user misses a click while slection multiple objects)
		if (InputManager::getSingletonPtr()->isKeyUp(SDL_SCANCODE_LCTRL))
		{
			//Remove AABB
			for (int i = 0; i < mSelectedObjects.size(); ++i)
			{
				mSelectedObjects.at(i)->showAABB(false);
			}

			if (!mSelectedObjects.empty())
			{
				//Reset to the old physics props
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					if (mSelectedObjects.at(i)->getType() == MovableObject::ENTITY)
					{
						static_cast<Entity*>(mSelectedObjects.at(i))->getRigidBody()->setMass(mPhysicsSavedProperties.at(i).mRigidBodyMass, mPhysicsSavedProperties.at(i).mRigidBodyType);
					}
				}
			}

			mSelectedObjects.clear();
		}
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

	//Update the propeties of the window if we select a new object or if we move the object
	if ((ent != mPreviousSelectedObject) || (mDraggingAxis_X || mDraggingAxis_Y || mDraggingAxis_Z))
	{
		//Create the UI Window
		mEditorDisplayer->createUIWindow("entityWindow_WorldEditor", 400, 400, "entitySelected.html")->setPosition_NDC(glm::vec2(-0.65, 0.45));
		mEditorDisplayer->getWindow("entityWindow_WorldEditor")->setVisible(true);

		glm::vec3 position, orientation, scale;
		position = ent->getPosition();
		orientation = ent->getOrientation_Euler();
		scale = ent->getScale();
		//Name
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "entName", "innerHTML", ent->getName());
		//Position
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textP1", "value", position.x);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textP2", "value", position.y);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textP3", "value", position.z);
		//Orientation
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textO1", "value", orientation.x);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textO2", "value", orientation.y);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textO3", "value", orientation.z);
		//Scale
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textS1", "value", scale.x);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textS2", "value", scale.y);
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textS3", "value", scale.z);
		//Physics
		mEditorDisplayer->setPropertyToWindow("entityWindow_WorldEditor", "textMass", "value", mPhysicsSavedProperties.at(0).mRigidBodyMass); //old mass, because now is 0 (kinetic), of the first object

		mPreviousSelectedObject = ent;
	}
}

void WorldEditor::drawAxis(Entity* firstEntity)
{
	//axis
	Entity* Xent = mSceneManager->createEntity("Xaxis_WorldEditor");
	Entity* Yent = mSceneManager->createEntity("Yaxis_WorldEditor");
	Entity* Zent = mSceneManager->createEntity("Zaxis_WorldEditor");

	if (firstEntity != NULL)
	{

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


		SceneNode* Xnod = mSceneManager->getRootSceneNode()->createChildSceneNode("Xnod");
		SceneNode* Ynod = mSceneManager->getRootSceneNode()->createChildSceneNode("Ynod");
		SceneNode* Znod = mSceneManager->getRootSceneNode()->createChildSceneNode("Znod");

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
		Xent->setVisible(true);
		Xent->setMass(0.0, false);
		Xent->attachMaterial("XaxisMat");
		Xent->getMaterial()->setBaseColorV(glm::vec3(1.0, 0.0, 0.0));
		Xent->setPhysicsOn(false);

		Yent->setVisible(true);
		Yent->setMass(0.0, false);
		Yent->attachMaterial("YaxisMat");
		Yent->getMaterial()->setBaseColorV(glm::vec3(0.0, 0.0, 1.0));
		Yent->setPhysicsOn(false);

		Zent->setVisible(true);
		Zent->setMass(0.0, false);
		Zent->attachMaterial("ZaxisMat");
		Zent->getMaterial()->setBaseColorV(glm::vec3(0.0, 1.0, 0.0));
		Zent->setPhysicsOn(false);
	}
	else
	{
		Xent->setVisible(false);
		Yent->setVisible(false);
		Zent->setVisible(false);
	}
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
			//Move all the selected objects
			for (int i = 0; i < mSelectedObjects.size(); ++i)
			{
				mSelectedObjects.at(i)->getAttachedSceneNode()->translate(glm::vec3((mouse.x - mLastMousePos.x) / 10.0, 0.0, 0.0)); //The selected object is the axis so translate it
			}
		}
		else
		{
			if (uniformScaling)
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
				}
			}
			else
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0, 0.0, 0.0)); //The selected object is the axis so scale it
				}
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
			for (int i = 0; i < mSelectedObjects.size(); ++i)
			{
				mSelectedObjects.at(i)->getAttachedSceneNode()->translate(glm::vec3(0.0, -(mouse.y - mLastMousePos.y) / 10.0, 0.0)); //The selected object is the axis so translate it
			}
		}
		else
		{
			if (uniformScaling)
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
				}
			}
			else
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3(0.0, -(mouse.y - mLastMousePos.y) / 100.0, 0.0)); //The selected object is the axis so scale it
				}
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
			for (int i = 0; i < mSelectedObjects.size(); ++i)
			{
				mSelectedObjects.at(i)->getAttachedSceneNode()->translate(glm::vec3(0.0, 0.0, -(mouse.x - mLastMousePos.x) / 10.0)); //The selected object is the axis so translate it
			}
		}
		else
		{
			if (uniformScaling)
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3((mouse.x - mLastMousePos.x) / 100.0)); //The selected object in all axis
				}
			}
			else
			{
				for (int i = 0; i < mSelectedObjects.size(); ++i)
				{
					mSelectedObjects.at(i)->getAttachedSceneNode()->scale(glm::vec3(0.0, 0.0, -(mouse.x - mLastMousePos.x) / 100.0)); //The selected object is the axis so scale it
				}
			}
		}
		mLastMousePos = mouse;
	}

	static_cast<Entity*>(mSelectedObjects.at(0))->getRigidBody()->setTransforms(static_cast<Entity*>(mSelectedObjects.at(0))->getAttachedSceneNode());
}