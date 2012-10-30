#include "StdAfx.h"

#ifndef _SHADOW_COMPOSITOR_LISTENER_H_
#define _SHADOW_COMPOSITOR_LISTENER_H_

#include <OgreCompositor.h>

class ShadowCompositorListener : public Ogre::CompositorInstance::Listener
{
public:
	ShadowCompositorListener(Ogre::Camera* camera) : _camera(camera) {}
	// this callback we will use to modify SSAO parameters
    void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
        if (pass_id != 42) // not SSAO, return
            return;

        // calculate the far-top-right corner in view-space
        Ogre::Vector3 farCorner = _camera->getViewMatrix(true) * _camera->getWorldSpaceCorners()[4];

        // get the pass
        Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

        // get the vertex shader parameters
        Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
        // set the camera's far-top-right corner
        if (params->_findNamedConstantDefinition("farCorner"))
            params->setNamedConstant("farCorner", farCorner);

        // get the fragment shader parameters
        params = pass->getFragmentProgramParameters();
        // set the projection matrix we need
        static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
            0.5,    0,    0,  0.5,
            0,   -0.5,    0,  0.5,
            0,      0,    1,    0,
            0,      0,    0,    1);
        if (params->_findNamedConstantDefinition("ptMat"))
            params->setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * _camera->getProjectionMatrixWithRSDepth());
        if (params->_findNamedConstantDefinition("far"))
            params->setNamedConstant("far", _camera->getFarClipDistance());
	}

private:
	Ogre::Camera* _camera;
};

#endif