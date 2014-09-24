#ifndef SLOTRESOLVER_H
#define SLOTRESOLVER_H

//Base class for Slots class (so we can use pointers to them)
class SlotResolver
{
public:
	SlotResolver();
	~SlotResolver();

	virtual void execute() = 0;
};

#endif