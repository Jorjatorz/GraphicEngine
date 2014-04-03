#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>


template <class T>
class Singleton
{
private:
	Singleton(const Singleton<T> &); //copy constructor

	Singleton& operator=(const Singleton<T> &); //assing operator

protected:
	static T* mSingleton; //static instance

public:
	Singleton(void)
	{
		assert(!mSingleton); //just created once
		mSingleton = static_cast<T*>(this); //allocate the singleton
	}
	~Singleton(void)
	{
		assert(mSingleton); //just delete the created
		mSingleton = 0;
	}
	static T& getSingleton(void)
	{
		assert(mSingleton); //check it exists
		return *mSingleton;
	}

	static T* getSingletonPtr(void)
	{
		assert(mSingleton); //check it exists
		return mSingleton;
	}


};

//initiate the singleton
template <typename T>
T* Singleton<T>::mSingleton = 0;




#endif