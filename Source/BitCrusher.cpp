/*
  ==============================================================================

    BitCrusher.cpp
    Created: 11 Jan 2020 2:07:19pm
    Author:  comra

  ==============================================================================
*/

#include "BitCrusher.h"
#include "FXLoop.h"
#include "math.h"

BitCrusher::BitCrusher(FXLoop * fxLoop, int index)
	:Effect(fxLoop, "BitCrusher", index)
{
	// freq knob
	addAndMakeVisible(frequencyKnob);
	frequencyKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	frequencyKnob.setRange(0.0, 1.0);
	frequencyKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	frequencyKnob.setValue(0.5);
	frequencyKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};
	addAndMakeVisible(frequencyLabel);
	frequencyLabel.setText("Sampling time", dontSendNotification);
	frequencyLabel.attachToComponent(&frequencyKnob, false);
	frequencyLabel.setJustificationType(Justification::centred);

	// bit depth knob
	addAndMakeVisible(bitKnob);
	bitKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	bitKnob.setRange(0.0, 1.0);
	bitKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	bitKnob.setValue(0.5);
	bitKnob.setSkewFactorFromMidPoint(0.01);
	bitKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};
	addAndMakeVisible(bitLabel);
	bitLabel.setText("Bit resolution", dontSendNotification);
	bitLabel.attachToComponent(&bitKnob, false);
	bitLabel.setJustificationType(Justification::centred);

	// blend knob
	addAndMakeVisible(blendKnob);
	blendKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	blendKnob.setRange(0.0, 1.0);
	blendKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	blendKnob.setValue(0.5);
	blendKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};
	addAndMakeVisible(blendLabel);
	blendLabel.setText("Blend", dontSendNotification);
	blendLabel.attachToComponent(&blendKnob, false);
	blendLabel.setJustificationType(Justification::centred);

	// removeButton
	addAndMakeVisible(removeButton);
	removeButton.setButtonText("Remove");
	removeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
	removeButton.onClick = [this, index] {
		pFXLoop->setEffect(index, "EmptyEffect");
	};
}

BitCrusher::~BitCrusher()
{
}

void BitCrusher::paint(Graphics &g)
{
	g.setColour(Colours::grey);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.0f, 2.0f);
	g.setColour((Colours::lightblue).withAlpha(0.4f));
	g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 5.0f);
	g.setColour(Colours::white);
	g.setFont(22.0f);
	g.drawText("Bit Crusher", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text
}

void BitCrusher::resized()
{
	auto area = getLocalBounds();
	int CONTROL_PANEL_SIZE = getWidth() / 4;
	area.removeFromTop(getHeight() / 4);
	frequencyKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));
	bitKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));
	blendKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 3));

	removeButton.setBounds(getWidth()*(9.0f / 10.0f), 0, getWidth()*(10.0f / 100.0f), getHeight() / 5.0f);
}

void BitCrusher::processSignal(AudioSampleBuffer * processedBuffer)
{
	auto numChannels = processedBuffer->getNumChannels();
	auto numSamples = processedBuffer->getNumSamples();
	auto frequency = frequencyKnob.getValue();
	int downSampling = round(frequencyKnob.getValue() * 50 + 2);
	int bit = bitKnob.getValue() * (32768 - 2)+ 2;
	auto processedLevel = blendKnob.getValue();
	auto cleanLevel = 1 - processedLevel;

	auto sampleVal = 0.0f;
	for (auto channel = 0; channel < numChannels; ++channel) {
		for (auto sample = 1; sample < numSamples; ++sample) {
			if (sample % downSampling == 0) {
				sampleVal = round((sampleVal + processedBuffer->getSample(channel, sample) / 2.0f) * bit);
				sampleVal /= bit;
			}
			else if (sample % downSampling == 1) {
				sampleVal = round(processedBuffer->getSample(channel, sample)* bit);
				sampleVal /= bit;
			}
			processedBuffer->setSample(channel, sample, sampleVal*processedLevel + processedBuffer->getSample(channel, sample)*cleanLevel);
		}
	}
}
