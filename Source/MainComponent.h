
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FXLoop.h"
#include "Bar.h"


class MainComponent   : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;	// wpisuje do bufora wyjœciowego próbki, które maj¹ zostaæ odtworzone
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;
	
	// stan interfejsu do otwierania plików
	enum UIState {
		WAITING_FOR_FILE,
		STOPPED,
		PLAYING
	};
	UIState uiState;

	// jeœli CLEAN - > odtwarzanie orygina³u, PROCESSED - > odtwarzanie z efektami
	enum FXState {
		CLEAN,
		PROCESSED
	};
	FXState fxState;

	void openFile();
	FXLoop fxLoop;		// obiekt agregujacy wszystkie efekty

	AudioSampleBuffer* getFileBuffer();		// zwraca bufor próbek z pliki
	AudioSampleBuffer* getProcessedBuffer();	// zwraca bufor próbek do przetwarzania

private:
	const int NUM_INPUT_CHANNELS = 2;
	const int NUM_OUTPUT_CHANNELS = 2;
	const int LEFT_CHANNEL = 0;
	const int RIGHT_CHANNEL = 1;
	const int EXTRA_SAMPLES = 48000; // extra samples for output buffer to apply delay

	// file 
	AudioFormatManager formatManager;
	AudioSampleBuffer fileBuffer;			// bufor próbek z pliku
	AudioSampleBuffer processedBuffer;		// bufor próbek do przetwarzania
	int filePosition;

	
	// UI
	Bar bar;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
