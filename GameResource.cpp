#include "GameResource.h"

/*
template <class T>
T GameResource<T>::getResource()
{
	return mResourceType;
}

template <class T>
std::string GameResource<T>::getFilePath()
{
	return mFilePath;
}

template <class T>
time_t GameResource<T>::getModificationTime()
{
	//will contain the information of the file
	struct stat fileInfo;

	//get the information of the file
	stat(mFilePath.c_str(), &fileInfo);

	//save the modification time
	lastModificationTime = fileInfo.st_mtime;

	return lastModificationTime;
}
*/