#include "StdAfx.h"

#include "LevelData.h"

//generic class for node and camera tracking.

class SplineTrack
{
public:

	void setWaypoints(LevelData::WaypointSet waypoints) { _waypoints = waypoints; }

	void generateCurve() { _curve = _waypoints.generateSpline(); }

private:
	LevelData::WaypointSet _waypoints;

	Ogre::SimpleSpline _curve;

	float _distance;
};