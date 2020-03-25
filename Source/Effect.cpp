
#include "../JuceLibraryCode/JuceHeader.h"
#include "Effect.h"
#include "FXLoop.h"

//==============================================================================
Effect::Effect(FXLoop* fxLoop, std::string name, int index)
{
	pFXLoop = fxLoop;
	this->name = name;
	this->index = index;
}


Effect::~Effect()
{
}

std::string Effect::getName()
{
	return name;
}

void Effect::setIndex(int index)
{
	this->index = index;
}


