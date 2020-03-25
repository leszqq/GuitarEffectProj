
#include "Bar.h"
#include "MainComponent.h"


Bar::Bar (MainComponent* mainComponent)
{
	// przycisk otwierania pliku
	pMainComponent = mainComponent;
	addAndMakeVisible(openButton);
	openButton.setButtonText("Open file");
	openButton.onClick = [this] { 
		pMainComponent->openFile();
		pMainComponent->uiState = pMainComponent->UIState::STOPPED;
		update();
	};

	// przycisk odtwarzania
	addAndMakeVisible(playButton);
	playButton.setButtonText("Play");
	playButton.setEnabled(false);
	playButton.onClick = [this] {
		pMainComponent->uiState = pMainComponent->UIState::PLAYING;
		update();
	};

	// przycisk stop
	addAndMakeVisible(stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setEnabled(false);
	stopButton.onClick = [this] {
		pMainComponent->uiState = pMainComponent->UIState::STOPPED;
		update();
	};

	// przycisk wyboru dŸwiêku czystego / przetworzonego
	addAndMakeVisible(applyEffectsButton);
	applyEffectsButton.setButtonText("Apply Effects");
	applyEffectsButton.setEnabled(false);
	applyEffectsButton.onClick = [this] {
		if(pMainComponent->fxState == pMainComponent->FXState::CLEAN){
			pMainComponent->fxLoop.processSignal();
			pMainComponent->fxState = pMainComponent->FXState::PROCESSED;
			applyEffectsButton.setButtonText("Remove effects");
		} 
		else if (pMainComponent->fxState == pMainComponent->FXState::PROCESSED) {
			pMainComponent->fxState = pMainComponent->FXState::CLEAN;
			applyEffectsButton.setButtonText("Apply effects");
		}
	};

    setSize (getWidth(), getHeight());
}

Bar::~Bar()
{

}

void Bar::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void Bar::resized()
{
	auto area = getLocalBounds();
	int WIDTH = getWidth()/8;

	openButton.setBounds(area.removeFromLeft(WIDTH));
	playButton.setBounds(area.removeFromLeft(WIDTH));
	stopButton.setBounds(area.removeFromLeft(WIDTH));
	applyEffectsButton.setBounds(area.removeFromLeft(WIDTH));
}


void Bar::update()
{
	if (pMainComponent->uiState == pMainComponent->UIState::STOPPED) {
		playButton.setEnabled(true);
		stopButton.setEnabled(false);
		applyEffectsButton.setEnabled(true);
	}
	else if (pMainComponent->uiState == pMainComponent->UIState::PLAYING) {
		playButton.setEnabled(false);
		stopButton.setEnabled(true);
		applyEffectsButton.setEnabled(true);
	}
}




