#include "WorldEditor.h"

#include "RayCast.h"
#include "MovableObject.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIDisplayer.h"

WorldEditor::WorldEditor(SceneManager* manager)
{
	mSceneManager = manager;
}


WorldEditor::~WorldEditor()
{
}

void WorldEditor::processWorldEditor()
{
	for (int i = 0; i < mSelectedObjects.size(); ++i)
	{
		mSelectedObjects.at(i)->showAABB(true);
	}

	if (mSelectedObjects.size() != 0)
	{
		mSceneManager->getCurrentDisplayer()->setPropertyToWindow("b", "windowtitle", "innerHTML", "Entity Editor - " + mSelectedObjects.at(0)->getName());
	}
	else
	{
		mSceneManager->getCurrentDisplayer()->setPropertyToWindow("b", "windowtitle", "innerHTML", "Entity Editor");
	}
}

void WorldEditor::selectObject_RayCast(glm::vec3 cameraPos, glm::vec3 mouseDir_World)
{
	RayCast mRay;
	mRay.executeRaySelectionCast(cameraPos, mouseDir_World * 1000.0f);

	if (mRay.hasHits())
	{
		if (InputManager::getSingletonPtr()->isKeyDown(SDL_SCANCODE_LCTRL))
		{
			mSelectedObjects.push_back(mRay.getHitObject());
		}
		else //Just one selection
		{
			//Remove AABB
			for (int i = 0; i < mSelectedObjects.size(); ++i)
			{
				mSelectedObjects.at(i)->showAABB(false);
			}

			mSelectedObjects.clear();

			mSelectedObjects.push_back(mRay.getHitObject());
		}
	}
	else
	{
		//Remove AABB
		for (int i = 0; i < mSelectedObjects.size(); ++i)
		{
			mSelectedObjects.at(i)->showAABB(false);
		}

		mSelectedObjects.clear();
	}
}