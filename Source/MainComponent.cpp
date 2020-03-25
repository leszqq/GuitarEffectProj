
#include "MainComponent.h"


MainComponent::MainComponent()
	: bar(this), fxState(CLEAN), fxLoop(this)
{
	addAndMakeVisible(bar);

	for (std::shared_ptr<Effect> e : fxLoop.getEffects())
	{
		addAndMakeVisible(e.get());
	}

	setSize(1024, 768);

	formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}


void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	switch (uiState) {
	case PLAYING:
	{
		// rendering sound directly from file or after modification depending on FXState (Clean or Distorted)
		auto outputSamplesToFill = bufferToFill.numSamples;
		auto outputStartSample = bufferToFill.startSample;
		while (outputSamplesToFill > 0) {
			auto fileSamplesRemaining = fileBuffer.getNumSamples() - filePosition;
			auto samplePacketSize = jmin(outputSamplesToFill, fileSamplesRemaining);

			for (auto channel = 0; channel < NUM_OUTPUT_CHANNELS; ++channel) {
				if (fxState == CLEAN)
					bufferToFill.buffer->copyFrom(channel, outputStartSample, fileBuffer, channel, filePosition, samplePacketSize);
				else if (fxState == PROCESSED) {
					bufferToFill.buffer->copyFrom(channel, outputStartSample, processedBuffer, channel, filePosition, samplePacketSize);
				}

			}
			outputSamplesToFill -= samplePacketSize;
			outputStartSample += samplePacketSize;
			filePosition += samplePacketSize;

			if (filePosition == fileBuffer.getNumSamples())
				filePosition = 0;
		}
		break;
	}
	case WAITING_FOR_FILE:
		bufferToFill.clearActiveBufferRegion();
		break;
	case STOPPED:
		bufferToFill.clearActiveBufferRegion();
		break;
	default:
		break;
	}
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	auto area = getLocalBounds();
	int barSize = 30;
	int spacing = 10;

	bar.setBounds(area.removeFromTop(barSize));
	area.reduce(spacing, spacing);
	float effectSize = (area.getHeight() - 3*spacing) / 4.0f;
	for (std::shared_ptr<Effect> e : fxLoop.getEffects())
	{
		e.get()->setBounds(area.removeFromTop(effectSize));
		area.removeFromTop(spacing);
	}
}

void MainComponent::openFile()
{
	shutdownAudio();

	FileChooser chooser("Select stereo WAV file", {}, "*.wav");

	if (chooser.browseForFileToOpen()) {
		auto file = chooser.getResult();

		std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

		if (reader.get() != nullptr) {
			if (reader->numChannels == NUM_INPUT_CHANNELS) {
				fileBuffer.setSize(NUM_INPUT_CHANNELS, (int)reader->lengthInSamples);
				processedBuffer.setSize(NUM_INPUT_CHANNELS, (int)reader->lengthInSamples + EXTRA_SAMPLES);
				reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true, true);
				uiState = UIState::STOPPED;
				filePosition = 0;
				setAudioChannels(0, NUM_OUTPUT_CHANNELS);
				fxLoop.processSignal();
			}
		}
	}
}

AudioSampleBuffer* MainComponent::getFileBuffer()
{
	return &fileBuffer;
}

AudioSampleBuffer * MainComponent::getProcessedBuffer()
{
	return &processedBuffer;
}




