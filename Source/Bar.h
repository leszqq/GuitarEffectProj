
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent;


class Bar  : public Component
{
public:
	Bar(MainComponent* mainComponent);
    ~Bar();

    void paint (Graphics& g) override;
    void resized() override;



private:
	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	TextButton applyEffectsButton;

	MainComponent* pMainComponent;

	void update();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bar)
};


