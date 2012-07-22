#include "StdAfx.h"

#include "SplineTrack.h"

#include <algorithm>

void CameraTrack::setAtStart()
{
	_currentTime = 0.0f;
	_distance = 0.0f;
	_camera->setPosition(_curve.interpolate(_distance));
}

void CameraTrack::update(float incrementInMillisecs)
{
	_currentTime += incrementInMillisecs;
	_distance = _currentTime / _targetTime;

	//update the camera position
	_camera->setPosition(_curve.interpolate(_distance));
}

void NodeTrack::setAtStart()
{
	_currentTime = 0.0f;
	_distance = 0.0f;
	_node->setPosition(_curve.interpolate(0.0f));
}

void NodeTrack::update(float incrementInMillisecs)
{
	_currentTime += incrementInMillisecs;
	_distance = _currentTime / _targetTime;

	if(_distance < 1.0f)
		_node->setPosition(_curve.interpolate(_distance));
}