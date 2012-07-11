//SERVER CODE
#include "StdAfx.h"

#include <windows.h>
#include <string>
#include <assert.h>
#include <cstdio>
#include <io.h>
#include <fcntl.h>

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

class Console : public Ogre::Singleton<Console>
{
public:
	Console(const std::string& name);

	//get handle,etc etc
	HWND getHandle();
	
	void positionConsole(int top,int left);
private:

};

#endif