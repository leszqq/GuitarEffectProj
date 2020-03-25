
#include "Delay.h"
#include "FXLoop.h"
#include "MainComponent.h"

Delay::Delay(FXLoop * fxLoop, int index)
	:Effect(fxLoop,"Delay", index)
{
	// time knob
	addAndMakeVisible(timeKnob);
	timeKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	timeKnob.setRange(0.0, 1.0);
	timeKnob.setTextBoxStyle(Slider::NoTextBox, true, 100, 20);
	timeKnob.setValue(0.5);
	timeKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};

	addAndMakeVisible(timeLabel);
	timeLabel.setText("time", dontSendNotification);
	timeLabel.attachToComponent(&timeKnob, false);
	timeLabel.setJustificationType(Justification::centred);

	// level knob
	addAndMakeVisible(levelKnob);
	levelKnob.setSliderStyle(Slider::SliderStyle::Rotary);
	levelKnob.setRange(0.0, 1.0);
	levelKnob.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	levelKnob.setValue(0.5);
	levelKnob.onDragEnd = [this, fxLoop] {
		fxLoop->processSignal();
	};

	addAndMakeVisible(levelLabel);
	levelLabel.setText("Level", dontSendNotification);
	levelLabel.attachToComponent(&levelKnob, false);
	levelLabel.setJustificationType(Justification::centred);


	// removeButton
	addAndMakeVisible(removeButton);
	removeButton.setButtonText("Remove");
	removeButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
	removeButton.onClick = [this, index, fxLoop] {
		pFXLoop->setEffect(index, "EmptyEffect");
	};
}

Delay::~Delay()
{
}

void Delay::paint(Graphics &g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.0f, 2.0f);
	g.setColour((Colours::lightyellow).withAlpha(0.2f));
	g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 5.0f);
	g.setColour(Colours::white);
	g.setFont(22.0f);
	g.drawText("Delay", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text
}

void Delay::resized()
{
	auto area = getLocalBounds();
	int CONTROL_PANEL_SIZE = getWidth() / 3;
	area.removeFromTop(getHeight() / 4);
	timeKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 2));
	levelKnob.setBounds(area.removeFromLeft(CONTROL_PANEL_SIZE / 2));

	removeButton.setBounds(getWidth()*(9.0f / 10.0f), 0, getWidth()*(10.0f / 100.0f), getHeight() / 5.0f);
}


void Delay::processSignal(AudioSampleBuffer * processedBuffer)
{
	auto numChannels = processedBuffer->getNumChannels();
	auto numSamples = processedBuffer->getNumSamples();
	int time = timeKnob.getValue()*44100*DELAY_FACTOR;
	float level = levelKnob.getValue()*LEVEL_FACTOR;


	for (auto channel = 0; channel < numChannels; ++channel) {
		for (auto sample = 0; sample < numSamples; ++sample) {
			if (sample > time) {
				processedBuffer->setSample(channel, sample, processedBuffer->getSample(channel, sample) + level * processedBuffer->getSample(channel, sample - time));
			}
		}
	}
}
