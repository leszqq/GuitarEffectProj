
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Effect.h"




class EmptyEffect    : public Effect
{
public:
    EmptyEffect(FXLoop* fxLoop, int index);
    ~EmptyEffect();

    void paint (Graphics&) override;
    void resized() override;
	void processSignal(AudioSampleBuffer * processedBuffer) override;

private:

	TextButton addButton;
	std::vector<std::shared_ptr<TextButton>> effectChooseButtons;

	std::vector<std::shared_ptr<Effect>> availableEffects;	// dostêpne efekty

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EmptyEffect)
};
