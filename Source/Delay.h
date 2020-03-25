
#pragma once
#include "Effect.h"

#include "../JuceLibraryCode/JuceHeader.h"


class Delay : public Effect

{
public:
	Delay(FXLoop* fxLoop, int index);
	~Delay();

	void paint(Graphics&) override;
	void resized() override;
	void processSignal(AudioSampleBuffer * processedBuffer) override;


private:
	Slider timeKnob;
	Label timeLabel;
	Slider levelKnob;
	Label levelLabel;
	TextButton removeButton;

	const float DELAY_FACTOR = 2.0;
	const float LEVEL_FACTOR = 0.4f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Delay)
};
