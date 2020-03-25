#include "FXLoop.h"
#include "MainComponent.h"
#include "Tremolo.h"
#include "Math.h"


Tremolo::Tremolo(FXLoop * fxLoop, int index)
	:Effect(fxLoop, "Tremolo", index)
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
	frequencyLabel.setText("Frequency", dontSendNotification);
	frequencyLabel.attachToComponent(&frequencyKnob, false);
	frequencyLabel.setJustificationType(Justification::centred);

	// depth knob
	addAndMakeVisible(depthKnob);
	depthKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	depthKnob.setRange(0.0, 1.0);
	depthKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	depthKnob.setValue(0.5);
	depthKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};
	addAndMakeVisible(depthLabel);
	depthLabel.setText("Depth", dontSendNotification);
	depthLabel.attachToComponent(&depthKnob, false);
	depthLabel.setJustificationType(Justification::centred);

	// removeButton
	addAndMakeVisible(removeButton);
	removeButton.setButtonText("Remove");
	removeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
	removeButton.onClick = [this, index] {
		pFXLoop->setEffect(index, "EmptyEffect");
	};
}

Tremolo::~Tremolo()
{
}

void Tremolo::paint(Graphics &g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::grey);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.0f, 2.0f);
	g.setColour((Colours::purple).withAlpha(0.8f));
	g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 5.0f);
	g.setColour(Colours::white);
	g.setFont(22.0f);
	g.drawText("Tremolo", getLocalBounds(),
		Justification::centred, true);
}

void Tremolo::resized()
{
	auto area = getLocalBounds();
	int CONTROL_PANEL_SIZE = getWidth() / 3;
	area.removeFromTop(getHeight() / 4);
	frequencyKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 2));
	depthKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 2));

	removeButton.setBounds(getWidth()*(9.0f / 10.0f), 0, getWidth()*(10.0f / 100.0f), getHeight() / 5.0f);
}

void Tremolo::processSignal(AudioSampleBuffer * processedBuffer)
{
	auto numChannels = processedBuffer->getNumChannels();
	auto numSamples = processedBuffer->getNumSamples();
	auto deltaFi = frequencyKnob.getValue()*2*3.14/FREQUENCY_FACTOR;
	auto depth = depthKnob.getValue();
	auto fi = 0.0;

	for (auto channel = 0; channel < numChannels; ++channel) {
		for (auto sample = 0; sample < numSamples; ++sample) {

			processedBuffer->setSample(channel, sample, processedBuffer->getSample(channel, sample)*(depth*0.5*sin(fi) + 0.5f) );
			fi += deltaFi;
		}
	}
}
