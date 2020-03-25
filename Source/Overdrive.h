
#pragma once
#include "Effect.h"

#include "../JuceLibraryCode/JuceHeader.h"


class Overdrive    : public Effect
					
{
public:
    Overdrive(FXLoop* fxLoop, int index);
    ~Overdrive();

    void paint (Graphics&) override;
    void resized() override;
	void processSignal(AudioSampleBuffer * processedBuffer) override;
	

private:
	Slider gainKnob;
	Label gainLabel;
	Slider blendKnob;
	Label blendLabel;
	Slider threshKnob;
	Label threshLabel;
	TextButton removeButton;

	const float GAIN_FACTOR = 5.0f;
	const float THRESHOLD_FACTOR = 0.5f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Overdrive)
};
