#pragma once
//#include "MainComponent.h"

#include "Effect.h"
#include "EmptyEffect.h"

class MainComponent;

class FXLoop
{
public:
	FXLoop(MainComponent* pMainComponent);
	~FXLoop();

	std::shared_ptr<Effect> getEffect(int index);
	std::vector<std::shared_ptr<Effect>> const& getEffects() const;

	std::vector < std::shared_ptr<Effect>> getAvailableEffects();
	void setEffect(int index, std::shared_ptr<Effect> effect);
	void setEffect(int index, std::string effectName);

	MainComponent* pMainComponent;

	void processSignal();
private:
	const int NUM_EFFECTS = 4;

	std::vector<std::shared_ptr<Effect>> effects;
	std::vector<std::shared_ptr<Effect>> availableEffects;
};