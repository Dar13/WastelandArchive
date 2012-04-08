//SERVER CODE
#include "StdAfx.h"

#include <windows.h>
#include <string>
#include <assert.h>
#include <cstdio>

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

typedef unsigned long ulong;
class VirtualConsole : public Ogre::Singleton<VirtualConsole>
{
	HANDLE near_end, //Our end of the pipe, where we'll write.
		far_end, //The other end.
		process; //The client.
public:
	bool good;
	VirtualConsole(const std::string &name,ulong color);
	~VirtualConsole();
	void put(const char *str,size_t size=0);
	void put(const std::string &str){
		this->put(str.c_str(),str.size());
	}
private:
	VirtualConsole(const VirtualConsole&);
	VirtualConsole& operator= (VirtualConsole&);
};

#endif