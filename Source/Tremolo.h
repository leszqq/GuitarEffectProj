
#pragma once
#include "Effect.h"

#include "../JuceLibraryCode/JuceHeader.h"


class Tremolo : public Effect

{
public:
	Tremolo(FXLoop* fxLoop, int index);
	~Tremolo();

	void paint(Graphics&) override;
	void resized() override;
	void processSignal(AudioSampleBuffer * processedBuffer) override;


private:
	Slider frequencyKnob;
	Label frequencyLabel;
	Slider depthKnob;
	Label depthLabel;
	TextButton removeButton;

	const float DELAY_FACTOR = 2.0;
	const float FREQUENCY_FACTOR = 44100.0f / 10.0f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tremolo)
};
