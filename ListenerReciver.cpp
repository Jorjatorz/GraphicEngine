#include "ListenerReciver.h"

#include "SlotResolver.h"
/*
ListenerReciver::ListenerReciver()
{
}
*/
/*
ListenerReciver::~ListenerReciver()
{
}*/
void ListenerReciver::executeAttachedFunction()
{
	mSlot->execute();
}