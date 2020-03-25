

#include "../JuceLibraryCode/JuceHeader.h"
#include "EmptyEffect.h"
#include "FXLoop.h"
#include "MainComponent.h"
#include <algorithm>

EmptyEffect::EmptyEffect(FXLoop* fxLoop, int index)
	: Effect(fxLoop, "EmptyEffect", index)
{
	// klawisze dodawania efektów
	for (std::shared_ptr<Effect> e : pFXLoop->getAvailableEffects()) {
		std::string name = e->getName();
		effectChooseButtons.push_back(std::move(std::make_unique<TextButton>(e->getName())));
		effectChooseButtons.at(effectChooseButtons.size() - 1)->onClick = [this, index, name] {
			pFXLoop->setEffect(index, name);
		};
	}

	addAndMakeVisible(addButton);
	addButton.setButtonText("Click to add new Effect");
	addButton.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgreen);
	
	addButton.onClick = [this] {
		addButton.setEnabled(false);
		addButton.setButtonText("");
		for (std::shared_ptr<TextButton> e : effectChooseButtons) {
			addAndMakeVisible(e.get());
		}
	};
}

EmptyEffect::~EmptyEffect()
{
}

void EmptyEffect::paint (Graphics& g)
{
    g.fillAll (Colours::green);   // clear the background
}

void EmptyEffect::resized()
{
	addButton.setBounds(0, 0, getWidth(), getHeight());
	
	auto area = getLocalBounds();
	int numOfChooseButtons = effectChooseButtons.size();
	int width = getWidth();
	for (std::shared_ptr<TextButton> e : effectChooseButtons) {
		e->setBounds(area.removeFromLeft(width / numOfChooseButtons));
	}
}

void EmptyEffect::processSignal(AudioSampleBuffer * processedBuffer)
{
}

