#include "StdAfx.h"

#include "LevelData.h"

#ifndef _SPLINE_TRACK_H_
#define _SPLINE_TRACK_H_

//generic class for node and camera tracking.

class SplineTrack
{
public:

	void setWaypoints(LevelData::WaypointSet waypoints) { _waypoints = waypoints; }
	LevelData::WaypointSet getWaypoints() { return _waypoints; }

	void generateCurve() { _curve = _waypoints.generateSpline(); }
	Ogre::SimpleSpline getCurve() { return _curve; }

	float getProgress() { return _distance; }
	
	void setTargetTimeLength(float timeInMillisecs = 1.0f) { _targetTime = timeInMillisecs; }

	virtual void update(float incrementInMillisecs) = 0;

protected:
	LevelData::WaypointSet _waypoints;

	Ogre::SimpleSpline _curve;

	float _distance;
	float _targetTime;
	float _currentTime;
};

class CameraTrack : public SplineTrack
{
public:
	void setCamera(Ogre::Camera* cam) { _camera = cam; }

	void setAtStart();
	void update(float incrementInMillisecs);

private:
	Ogre::Camera* _camera;
};

class NodeTrack : public SplineTrack
{
public:
	void setNode(Ogre::SceneNode* node) { _node = node; }

	void setAtStart();
	void update(float incrementInMillisecs);
private:
	Ogre::SceneNode* _node;
};

#endif