#include "StdAfx.h"

#ifndef _ANIMATIONBLENDER_H_
#define _ANIMATIONBLENDER_H_

class AnimationBlender
{
public:
	enum BlendingTransition
	{
		BlendSwitch,         // stop source and start dest
		BlendWhileAnimating,   // cross fade, blend source animation out while blending destination animation in
		BlendThenAnimate      // blend source to first frame of dest, when done, start dest anim
	};
 
private:
	Ogre::Entity *mEntity;
	Ogre::AnimationState *mSource;
	Ogre::AnimationState *mTarget;
 
	BlendingTransition mTransition;
 
	bool loop;
 
	
 
public: 
	Ogre::Real mTimeleft, mDuration;
 
	bool complete;
	
	AnimationBlender( Ogre::Entity* );
	AnimationBlender() {}
	~AnimationBlender() {}
	
	void init( const std::string& animation, bool l=true );
	
	void setEntity( Ogre::Entity* entity ) { mEntity = entity; }
	
	Ogre::AnimationState* getSource() { return mSource; }
	Ogre::AnimationState* getTarget() { return mTarget; }
	Ogre::Real getProgress() { return mTimeleft/ mDuration; }
 
	void blend( const std::string &animation, BlendingTransition transition, Ogre::Real duration, bool l=true );
	void addTime( Ogre::Real );
};

#endif