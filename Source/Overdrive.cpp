#include "Overdrive.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "FXLoop.h"
#include "MainComponent.h"

//==============================================================================
Overdrive::Overdrive(FXLoop* fxLoop, int index)
	:Effect(fxLoop, "Overdrive", index)
{
	// gain knob
	addAndMakeVisible(gainKnob);
	gainKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	gainKnob.setRange(0.0, 1.0);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	gainKnob.setValue(0.5);
	gainKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};
	
	addAndMakeVisible(gainLabel);
	gainLabel.setText("Gain", dontSendNotification);
	gainLabel.attachToComponent(&gainKnob, false);
	gainLabel.setJustificationType(Justification::centred);

	// blend knob
	addAndMakeVisible(blendKnob);
	blendKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	blendKnob.setRange(0.0, 1.0);
	blendKnob.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	blendKnob.setValue(0.5);
	blendKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};

	addAndMakeVisible(blendLabel);
	blendLabel.setText("Blend", dontSendNotification);
	blendLabel.attachToComponent(&blendKnob, false);
	blendLabel.setJustificationType(Justification::centred);

	// threshold knob
	addAndMakeVisible(threshKnob);
	threshKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	threshKnob.setRange(0.0, 1.0);
	threshKnob.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	threshKnob.setValue(0.5);
	threshKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};

	addAndMakeVisible(threshLabel);
	threshLabel.setText("Threshold", dontSendNotification);
	threshLabel.attachToComponent(&threshKnob, false);
	threshLabel.setJustificationType(Justification::centred);


	// removeButton
	addAndMakeVisible(removeButton);
	removeButton.setButtonText("Remove");
	removeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
	removeButton.onClick = [this, index, fxLoop] {
		pFXLoop->setEffect(index, "EmptyEffect");
	};
}

Overdrive::~Overdrive()
{
}

void Overdrive::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::grey);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.0f, 2.0f);
	g.setColour((Colours::darkred).withAlpha(0.8f));
	g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 5.0f);
    g.setColour (Colours::white);
    g.setFont (22.0f);
    g.drawText ("Overdrive", getLocalBounds(),
                Justification::centred, true); 
}

void Overdrive::resized()
{
	auto area = getLocalBounds();
	int CONTROL_PANEL_SIZE = getWidth() / 3;
	area.removeFromTop(getHeight() / 4);
	gainKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));
	blendKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));
	threshKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));

	removeButton.setBounds(getWidth()*(9.0f / 10.0f), 0, getWidth()*(10.0f/100.0f), getHeight() / 5.0f);
}



void Overdrive::processSignal(AudioSampleBuffer * processedBuffer)
{
	auto numChannels = processedBuffer->getNumChannels();
	auto numSamples = processedBuffer->getNumSamples();
	auto gain = 1.0f + gainKnob.getValue()*GAIN_FACTOR;
	auto distLevel = blendKnob.getValue();
	auto cleanLevel = 1 - distLevel;
	auto threshold = threshKnob.getValue()*THRESHOLD_FACTOR;

	for (auto channel = 0; channel < numChannels; ++channel) {
		for (auto sample = 0; sample < numSamples; ++sample) {
			float samp = gain * processedBuffer->getSample(channel, sample);
			if (samp > threshold) {
				samp = threshold;
			}
			//else if (samp < -threshold) {
			//	samp = -threshold;
			//}
			processedBuffer->setSample(channel,sample , samp*distLevel + (processedBuffer->getSample(channel, sample))*cleanLevel);
		}
	}
}
