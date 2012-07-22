#include "StdAfx.h"

#include "CameraTrack.h"

#include <algorithm>

void CameraTrack::generateCurve()
{
	_curve = _waypoints.generateSpline();
}

void CameraTrack::setAtStart()
{
	_currentTime = 0.0f;
	_distance = 0.0f;
}

void CameraTrack::update(float incrementInMillisecs)
{
	_currentTime += incrementInMillisecs;
	_distance = _currentTime / _targetTime;

	//update the camera position
	Ogre::Vector3 nPos = _curve.interpolate(_distance);
	_camera->setPosition(nPos);
}