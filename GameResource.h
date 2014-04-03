#ifndef GAMERESOURCE_H
#define GAMERESOURCE_H

#include "Definitions.h"

#include <string>
//includes for modification time
#include <ctime>
#include <sys/stat.h>

template <class T>
class GameResource
{
public:
	//inlined constructors and destructor
	GameResource(void)
	{
		mFilePath = "undefinedPath";
		mResourceType = 0;
		lastModificationTime[0] = 0;
		lastModificationTime[1] = 0;
	}
	//constructor
	GameResource(std::string filePath, T resourceType)
	{
		mFilePath = filePath;
		mResourceType = resourceType;
		lastModificationTime[0] = 0;
		lastModificationTime[1] = 0;
	}

	~GameResource(void)
	{
		//delete the resource if establized
		if(mResourceType != 0)
			delete mResourceType;
	}

	//return the resource
	T getResource()
	{
		return mResourceType;
	}
	//return the file path
	std::string getFilePath()
	{
		return mFilePath;
	}

private:
	//file path
	std::string mFilePath;
	//type of resource
	T mResourceType;
	//time when the file was modified
	time_t lastModificationTime[2];

	friend class ResourceManager;
};

#endif