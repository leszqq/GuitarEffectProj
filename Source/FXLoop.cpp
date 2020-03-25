#include "FXLoop.h"
#include "Overdrive.h"
#include "Delay.h"
#include "MainComponent.h"
#include "Tremolo.h"
#include "BitCrusher.h"

FXLoop::FXLoop(MainComponent* pMainComponent)
{
	this->pMainComponent = pMainComponent;
	// tu nale¿y dodaæ nowe efekty
	availableEffects.push_back(std::make_shared<Overdrive>(this, 0));
	availableEffects.push_back(std::make_shared<BitCrusher>(this, 1));
	availableEffects.push_back(std::make_shared<Delay>(this, 2));
	availableEffects.push_back(std::make_shared<Tremolo>(this, 3));
	for (int i = 0; i < NUM_EFFECTS; i++)
		effects.push_back(std::move(std::make_shared<EmptyEffect>(this, i)));
}

FXLoop::~FXLoop()
{
}

std::shared_ptr<Effect> FXLoop::getEffect(int index)
{
	return effects.at(index);
}

std::vector<std::shared_ptr<Effect>> const& FXLoop::getEffects() const
{
	return effects;
}


std::vector<std::shared_ptr<Effect>> FXLoop::getAvailableEffects() 
{
	return availableEffects;
}


void FXLoop::setEffect(int index, std::shared_ptr<Effect> effect)
{
	effects.at(index) = effect;
	pMainComponent->addAndMakeVisible(getEffect(index).get());
	pMainComponent->resized();
}

void FXLoop::setEffect(int index, std::string effectName)
{
	// po dodaniu nowego efektu nale¿y rozszerzyæ instrukcje warunkow¹ o nazwê nowego efektu
	effects.at(index) = nullptr;
	if (effectName == "Overdrive")
		effects.at(index) = std::make_shared <Overdrive>(this, index);
	else if (effectName == "BitCrusher")
		effects.at(index) = std::make_shared <BitCrusher>(this, index);
	else if (effectName == "Delay")
		effects.at(index) = std::make_shared <Delay>(this, index);
	else if (effectName == "Tremolo")
		effects.at(index) = std::make_shared <Tremolo>(this, index);
	else if (effectName == "EmptyEffect") {
		effects.at(index) = std::make_shared <EmptyEffect>(this, index);

	}
	processSignal();

	pMainComponent->addAndMakeVisible(getEffect(index).get());
	pMainComponent->resized();
}

// wywo³uje metodê processSignal() na ka¿dym efekcie po kolei
void FXLoop::processSignal()
{
	AudioSampleBuffer* fileBuffer = pMainComponent->getFileBuffer();
	AudioSampleBuffer* processedBuffer = pMainComponent->getProcessedBuffer();
	for (int channel = 0; channel < processedBuffer->getNumChannels(); channel++) {
		for (int sample = 0; sample < fileBuffer->getNumSamples(); sample++) {
			processedBuffer->setSample(channel, sample, fileBuffer->getSample(channel, sample));
		}
	}

	for (int i = 0; i < NUM_EFFECTS; i++) {
		effects.at(i)->processSignal(processedBuffer);
	}
}

