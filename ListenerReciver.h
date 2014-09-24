#ifndef LISTENERRECIVER_H
#define LISTENERRECIVER_H

#include "Definitions.h"

//Holds the information of the objects that wants to recive information from listeners
class ListenerReciver
{
public:
	template<typename T>
	ListenerReciver(void (T::*func)(), T* obj)
	{
		mSlot = new MemberFunctionSlot<T>(func, obj);
	}
	~ListenerReciver()
	{
		delete mSlot;
	}

	void executeAttachedFunction();


private:
	SlotResolver* mSlot;

};

#endif