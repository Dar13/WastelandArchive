#include "StdAfx.h"

#include "Arena_Tutorial.h"
#include "GameManager.h"
#include "debug\console.h"
#include "Utility.h"
#include <boost\lexical_cast.hpp>

ArenaTutorial::ArenaTutorial()
{
	_camera = 0;
	_scene = 0;
	_rootNode = 0;
	_stateShutdown = false;
	_view = 0;
	_deltaTime = 0;
	_oldTime = 0;
}

void ArenaTutorial::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	//physics setup
	_physics.reset(new PhysicsManager() );
	_physics->Setup();
	_physics->setGravity(btVector3(0.0f,-9.8f,0.0f));

	//using a list instead of hardcoded files
	std::auto_ptr<list_t> objList = list("resource\\xml\\arena_list.xml");
	for(list_t::file_const_iterator itr = objList.get()->file().begin(); itr != objList.get()->file().end(); ++itr)
	{
		std::string tmp = (*itr);
		_pairs.push_back(GameManager::createObject(_scene,tmp,_physics.get(),Graphics));
	}

	_camera->setPosition(Ogre::Vector3(-1,1.9f,0));
	_camera->setNearClipDistance(.001f);
	_camera->setFarClipDistance(1000.0f);
	_camera->lookAt(0,1.8f,0);

	_player.reset(new Player());
	
	//set the camera aspect ratio
	_camera->setAspectRatio(16.0f/9.0f);
	_camera->setPolygonMode(Ogre::PM_WIREFRAME);

	//let's try out the character controller
	_controller.reset(new CharacterController(_camera,_camera->getPosition(),_physics->getWorld(),Graphics ) );
	//since we're using the character controller, should also lock the mouse.
	Input->setMouseLock(true);

	//let's setup the EWS system
	_ews.reset(new EWSManager(_scene));

	//attempt to make a door out of a hinge constraint in bullet.
	//get the last ogrebulletpair
	OgreBulletPair hinge1 = _pairs.at(_pairs.size()-2);
	OgreBulletPair level = _pairs.at(1);
	hinge1.btBody->setActivationState(DISABLE_DEACTIVATION);
	btHingeConstraint* hingeDoor;
	hingeDoor = new btHingeConstraint(*level.btBody,*hinge1.btBody,btVector3(20.5f,5.0f,-5.0f),btVector3(0.0f,0.0f,-5.0f),btVector3(0,1,0),btVector3(0,1,0));
	_physics->getWorld()->addConstraint(hingeDoor,true);
	hingeDoor->setLimit(-0.1f,SIMD_PI / 2);
	_constraints.push_back(hingeDoor);

	/*
	btHinge2Constraint* hinge;
	//hinge = _physics->createHingeConstraint(level.btBody,hinge1.btBody,btVector3(20.0f,5.0f,-5.0f),btVector3(-5.0f,0.0f,.25f),btVector3(0,1,0),btVector3(0,1,0));
	hinge = new btHinge2Constraint(*level.btBody,*hinge1.btBody,btVector3(20,5,-5),btVector3(0,1,0),btVector3(1,0,0));
	_physics->getWorld()->addConstraint(hinge,true);
	_constraints.push_back(hinge);
	*/

	/*
	btSliderConstraint* door;
	btTransform frameA;
	frameA = btTransform::getIdentity();
	frameA.setOrigin(btVector3(20.0f,5.0f,-5.0f));
	btTransform frameB;
	frameB = btTransform::getIdentity();
	frameA.setOrigin(btVector3(20.0f,5.0f,-5.0f));
	door = new btSliderConstraint(*level.btBody,*hinge1.btBody,frameA,frameB,true);
	_constraints.push_back(door);
	_physics->getWorld()->addConstraint(door,true);
	*/
	
	//hinge1.btBody->setGravity(btVector3(0.0f,0.0f,0.0f));

	_physics->setDebugDrawer(new CDebugDraw(_scene,_physics->getWorld()));
}

int ArenaTutorial::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_stateShutdown=false;
	Ogre::SceneNode* tmpNode = (Ogre::SceneNode*)_scene->getRootSceneNode()->getChild("nodetestSphere");

	OgreTransform playerTransform;

	btHingeConstraint* hinge = static_cast<btHingeConstraint*>(_constraints[0]);
	btRigidBody* hingeBody = _pairs.at(_pairs.size()-2).btBody;
	
	float time;
	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//current time
		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		//getting the delta time
		_deltaTime = time - _oldTime;
		//setting the old time
		_oldTime = time;

		//the state shutdown based on player input(?)
		_stateShutdown = Input->Update(true);

		if(Input->isMBPressed(OIS::MB_Left))
		{
			//hingeBody->applyTorqueImpulse(btVector3(0.0f,500.0f,0.0f));
			hinge->enableAngularMotor(true,3.0f,5.0f);
		}

		//Update the character controller
		_controller->update(_deltaTime,Input,playerTransform);

		//Update Player-specific stuff
		_player->Update(Input,_physics.get(),_ews.get(),playerTransform);

		//Update the EWS system
		_ews->Update(100,static_cast<int>(time),Input->isCFGKeyPressed(ENVWARNSYS),playerTransform);

		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::UpdateManagers(Graphics,_physics.get(),_deltaTime))
			_stateShutdown = true;

		//static_cast<btHingeConstraint*>(_constraints[0])->setMotorTarget(Ogre::Math::TWO_PI,Ogre::Math::PI/360.0f);
		VirtualConsole::getSingleton().put(boost::lexical_cast<std::string,btScalar>(hinge->getHingeAngle())+'\n');
	}

	//no matter what, end the program after this state. **TESTING ONLY**
	return END;
}

//clean-up of state
void ArenaTutorial::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//undo what I set in OIS
	Input->setMouseLock(false);

	//clean up what you initialized in the Setup() function.
	Graphics->getRenderWindow()->removeAllViewports();
	
	//Destroy the camera, since we've taken care of the viewport.
	_scene->destroyCamera(_camera);
	//Destroy all SceneNodes, Entites, etc.
	_scene->clearScene();

	//Set to null.
	_rootNode=0;

	//Clear the vectors
	_nodes.clear();
	_entities.clear();

	_constraints.clear();

	//cleaning up state-specific pointers.
	//since all pointers are auto_ptr members, then they will be deleted upon class destruction.

	//Destroy the scene manager.
	Graphics->getRoot()->destroySceneManager(_scene);
}