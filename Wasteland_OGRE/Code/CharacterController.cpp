#include "StdAfx.h"
#include "CharacterController.h"
#include "Utility.h"
#include "debug\print.h"

void CharacterController::create(Ogre::Camera* camera,const Ogre::Vector3& initialPosition,btDiscreteDynamicsWorld* phyWorld,GraphicsManager* Graphics)
{
	cCamera = camera;
	_world = phyWorld; //saves having to pass in PhyManager pointers

	btTransform initial;
	initial.setIdentity();
	initial.setOrigin(Utility::convert_OgreVector3(initialPosition));

	//collision detection
	cGhostObject = new btPairCachingGhostObject();
	cGhostObject->setWorldTransform(initial);

	//collision shape
	phyWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btScalar charHeight = 1.9f;
	btScalar charWidth = .75f;
	btConvexShape* capsule = new btCapsuleShape(charWidth,charHeight);
	cGhostObject->setCollisionShape(capsule);
	cGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	//setting up the kinematic controller
	//reusing variable(charHeight). Here it means stepHeight
	charHeight = .35f;
	cController = new btKinematicCharacterController(cGhostObject,capsule,charHeight);

	//adding ghost/controller to physics world
	phyWorld->addCollisionObject(cGhostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	phyWorld->addAction(cController);

	cNode = cCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode("characterController");
	//NO! Gets rid of bullet physics initial positioning.
	//cNode->setPosition(cCamera->getPosition());
	cNode->setPosition(initialPosition); //NOT TESTED
	cNode->setOrientation(cCamera->getOrientation());
	cCamera->setPosition(0,0,0);
	cNode->attachObject(cCamera);

	//Ogre::SceneNode* tmpNode = Graphics->createSceneNode(cCamera->getSceneManager(),object("resource\\xml\\test_box.xml").release(),cNode);
	//tmpNode->setPosition(5.0f,1.0f,0.0f);

	//for some reason, this expects a positive value.
	//so make the global bullet gravity its inverse.
	//Confusing, eh?
	cController->setGravity(-phyWorld->getGravity().y());
	cController->setJumpSpeed(btScalar(6.0f));

	return;
}

void CharacterController::update(float physicsTimeElapsed,InputManager* inputManager,OgreTransform& transform)
{
	btTransform cTransform = cGhostObject->getWorldTransform();
	/*btVector3 forwardDirection = cTransform.getBasis()[0]; forwardDirection.normalize();
	btVector3 upDirection = cTransform.getBasis()[1]; upDirection.normalize();
	btVector3 strafeDirection = cTransform.getBasis()[2]; strafeDirection.normalize();*/
	btVector3 forwardDirection = cTransform.getBasis()[2]; forwardDirection.normalize();
	btVector3 upDirection = cTransform.getBasis()[1]; upDirection.normalize();
	btVector3 strafeDirection = cTransform.getBasis()[0]; strafeDirection.normalize();
	btVector3 walkDirection; walkDirection.setZero();

	//velocity = meters per second * kilometers per hour
	btScalar walkVel = 2.0f * 4.0f;

	Ogre::Vector3 direction; Ogre::Quaternion rotation;
	if(inputManager->isCFGKeyPressed(FORWARD))
	{
		walkDirection -= forwardDirection;
	}
	if(inputManager->isCFGKeyPressed(BACKWARD))
	{
		walkDirection += forwardDirection;
	}
	if(inputManager->isCFGKeyPressed(RIGHT))
	{
		walkDirection += strafeDirection;
	}
	if(inputManager->isCFGKeyPressed(LEFT))
	{
		walkDirection -= strafeDirection;
	}
	if(inputManager->isCFGKeyPressed(JUMP))
	{
		cController->jump();
	}
	if(inputManager->isCFGKeyPressed(SPRINT))
	{
		walkVel = 2.0f * 6.0f;
	}
	
	//velocity.
	//Bullet goes by seconds, not milliseconds.
	btScalar walkSpd = walkVel * (physicsTimeElapsed / 1000.0f);

	//get direction vector.
	direction = Utility::convert_btVector3(walkDirection);
	rotation = cNode->getOrientation();
	rotation.z = 0.0f;
	//direction is normalized(should be). NOT FINISHED.
	direction = rotation * (rotation * direction);
	walkDirection = Utility::convert_OgreVector3(direction);

	//**
	//Mouse look
	//**
	int mmx,mmy;
	mmx = inputManager->getMouseMoveX();
	mmy = inputManager->getMouseMoveY();

	//*
	//y-axis
	//*
	//current rotation matrix
	btMatrix3x3 yorn = cTransform.getBasis();
	//multiply rotation by quaternion representing rotation increment around y-axis.
	//0.007 is a bit sensitive.
	yorn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),(-mmx * 0.01f)));
	//set rotation of collision detector
	cGhostObject->getWorldTransform().setBasis(yorn);

	//*
	//x-axis
	//*
	//reset rotation variable
	rotation = Ogre::Quaternion::ZERO;
	//angle increment based on mouse movement.
	Ogre::Radian angle;
	//was 0.007, too sensitive. Scaled it back some.
	angle = (-mmy) * 0.01f;

	//generate quaternion from mouse move angle and x-axis unit vector
	//rotation.FromAngleAxis(angle,Ogre::Vector3::UNIT_Z);
	rotation.FromAngleAxis(angle,Ogre::Vector3::UNIT_X);

	//Actual movement and stuff
	cController->setWalkDirection(walkDirection * walkSpd);

	//positioning the ogre node
	Ogre::Quaternion ogreRot;
	ogreRot = Utility::convert_btQuaternion(cGhostObject->getWorldTransform().getRotation());
	Ogre::Vector3 ogrePos = Utility::convert_btVector3(cGhostObject->getWorldTransform().getOrigin());

	cNode->setPosition(ogrePos);
	Ogre::Quaternion zRot = cNode->getOrientation();
	//getting rid of x and y axis rotation.
	//zRot.x = 0;
	zRot.y = 0;
	zRot.z = 0;

	//**
	//Look restrictions
	//**
	//check to rotate
	bool extraRotate = true;
	//reusing ogrePos
	//restricts looking up.
	ogrePos = Ogre::Vector3(0,5,0) + cNode->_getDerivedPosition();
	if(cCamera->isVisible(ogrePos) && (angle.valueRadians() > 0))
	{
	//	extraRotate = false;
	}
	//restricts looking down.
	//needs to be adjusted. NOT FINISHED
	ogrePos = Ogre::Vector3(0,0,0) + cNode->_getDerivedPosition();
	if(cCamera->isVisible(ogrePos) && (angle.valueRadians() < 0))
	{
	//	extraRotate = false;
	}

	cNode->setOrientation(ogreRot);
	//cNode->getChild(0)->setOrientation(ogreRot);
	cNode->rotate(zRot);
	//cNode->getChild(0)->rotate(zRot);
	//cCamera->setOrientation(ogreRot);
	//cCamera->rotate(zRot);
	if(extraRotate)
	{
		cNode->rotate(rotation);
		//cNode->getChild(0)->rotate(rotation);
		//cCamera->rotate(rotation);
	}

	transform.orientation = cNode->getOrientation();
	transform.position = cCamera->getDerivedPosition();
	transform.direction = cCamera->getDerivedDirection();
}

CharacterController::~CharacterController()
{
	//have to remove the controller and ghost object from the physics world.
	_world->removeAction(cController);
	_world->removeCollisionObject(cGhostObject);
	delete cController;
	delete cGhostObject;
}