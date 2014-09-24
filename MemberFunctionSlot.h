#ifndef MEMBERFUNCTIONSLOT_H
#define MEMBERFUNCTIONSLOT_H

#include "Definitions.h"
#include "SlotResolver.h"

template <typename T>
class MemberFunctionSlot : public SlotResolver
{
public:
	//Member function slot type.
	typedef void(T::*MemberFunctionType)();

	MemberFunctionSlot(MemberFunctionType func, T* obj)
	{
		mFunction = func;
		mObject = obj;
	}
	~MemberFunctionSlot();

	void execute()
	{
		(mObject->*mFunction)();
	}

private:
	MemberFunctionType mFunction;
	T* mObject;
};

#endif