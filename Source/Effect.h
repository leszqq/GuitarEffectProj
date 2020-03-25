#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

class FXLoop;


class Effect    : public Component
{
public:
    Effect(FXLoop* fxLoop, std::string name, int index);	// nazwa efektu jest istotna. Na jej podstawie tworzone s¹ nowe instancje obiektu
    ~Effect();

    void paint (Graphics&) override = 0;
    void resized() override = 0;
	
	virtual void processSignal(AudioSampleBuffer * processedBuffer) = 0;	// metoda realizujaca przetwarzanie próbek sygna³u

	std::string getName();
	void setIndex(int index);												// ustawianie miejsca w pêtli efektów FXLoop
private:

protected:
	FXLoop* pFXLoop;
	std::string name;
	int index;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Effect)
};
