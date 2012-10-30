#include "StdAfx.h"

#ifndef _SHADOW_LISTENER_H_
#define _SHADOW_LISTENER_H_

#include <OgreSceneManager.h>

class ShadowListener : public Ogre::SceneManager::Listener
{
public:
	void shadowTextureCasterPreViewProj(Ogre::Light* light,Ogre::Camera* camera, size_t)
	{
		float range = light->getAttenuationRange();
		camera->setNearClipDistance(.01f);
		camera->setFarClipDistance(range);
	}

	void shadowTexturesUpdated(size_t){}
	void shadowTextureReceiverPreViewProj(Ogre::Light*,Ogre::Frustum*) {}
	void preFindVisibleObjects(Ogre::SceneManager*,Ogre::SceneManager::IlluminationRenderStage,Ogre::Viewport*){}
	void postFindVisibleObject(Ogre::SceneManager*,Ogre::SceneManager::IlluminationRenderStage,Ogre::Viewport*){}
};

#endif