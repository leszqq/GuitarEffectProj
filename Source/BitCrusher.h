
#pragma once
#include "Effect.h"

#include "../JuceLibraryCode/JuceHeader.h"


class BitCrusher : public Effect

{
public:
	BitCrusher(FXLoop* fxLoop, int index);
	~BitCrusher();

	void paint(Graphics&) override;
	void resized() override;
	void processSignal(AudioSampleBuffer * processedBuffer) override;


private:
	Slider frequencyKnob;
	Label frequencyLabel;
	Slider bitKnob;
	Label bitLabel;
	Slider blendKnob;
	Label blendLabel;

	TextButton removeButton;

	

	const float DELAY_FACTOR = 10.0f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitCrusher)
};
