#include "StdAfx.h"

#ifndef _CAMERA_TRACK_H_
#define _CAMERA_TRACK_H_

#include "LevelData.h"

class CameraTrack
{
public:
	void setCamera(Ogre::Camera* cam) { _camera = cam; }

	void generateCurve();
	Ogre::SimpleSpline getCurve() { return _curve; }

	void setWaypoints(LevelData::WaypointSet waypoints) { _waypoints = waypoints; }
	LevelData::WaypointSet getWaypoints() { return _waypoints; }

	float getProgress() { return _distance; }
	
	void setTargetTimeLength(float timeInMillisecs = 1.0f) { _targetTime = timeInMillisecs; }

	void setAtStart();
	void update(float increment);

private:
	Ogre::SimpleSpline _curve;
	float _currentTime;
	float _targetTime;
	float _distance;

	LevelData::WaypointSet _waypoints;

	Ogre::Camera* _camera;
};

#endif