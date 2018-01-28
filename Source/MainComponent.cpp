
#include "../JuceLibraryCode/JuceHeader.h"


class MainContentComponent   : public AudioAppComponent,
                               private OSCReceiver,
                               private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>,
                               public Slider::Listener,
                               private Label::Listener,
                               public Button::Listener

{
public:
    MainContentComponent() : state(Instrument)
    {
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        // specify a UDP port to connect to with OSCReceiver
        connect(6969);
        // listen to OSC messages at this address at port 6969
        addListener(this, "/theremin/midi");
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        waves[0] = { "sine"};
        waves[1] = {"square"};
        currentWave = waves[0];
        enableDistortion = false;
        distortionAlpha = 1;
        enableWaves = true;
        
        addAndMakeVisible(sensor);
        sensor.setText(String(frequency), dontSendNotification);
        sensor.setFont( (Font (20.00f, Font::plain).withTypefaceStyle("Bold")));
        sensor.setBounds(getWidth() / 2, getHeight() / 3 - 10, 100, 50);
        
        addAndMakeVisible(stateButton);
        stateButton.setBounds(100, 700, 100, 100);
        stateButton.setButtonText(TRANS("Switch to FX Controller"));
        stateButton.addListener(this);

        addAndMakeVisible(freqSlider);
        freqSlider.setRange(10, 22000);
        freqSlider.setTextValueSuffix(" Hz");
        freqSlider.setValue(220.0);
        freqSlider.addListener(this);
        freqSlider.setSkewFactorFromMidPoint(500);
        
        addAndMakeVisible (sineWaveButton);
        sineWaveButton.setButtonText (TRANS("Sine Wave"));
        sineWaveButton.addListener (this);
        sineWaveButton.setBounds (0, 20, 380, 100);
        sineWaveButton.setColour(TextButton::buttonColourId, Colour(46, 204, 113));
        
        addAndMakeVisible (squareWaveButton);
        squareWaveButton.setButtonText (TRANS("Square Wave"));
        squareWaveButton.addListener (this);
        squareWaveButton.setBounds(420, 20, 380, 100);
        squareWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
        
        addAndMakeVisible(enabledWaveButton);
        enabledWaveButton.setButtonText (TRANS("Wave Synthesis ON"));
        enabledWaveButton.addListener(this);
        enabledWaveButton.setBounds(375, 5, 50, 20);
        enabledWaveButton.setColour(TextButton::buttonColourId, Colour(46, 204, 113));
        
        addAndMakeVisible (sensorReading);
        sensorReading.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        sensorReading.setJustificationType (Justification::centredLeft);
        sensorReading.setEditable (false, false, false);
        sensorReading.setColour (TextEditor::textColourId, Colours::black);
        sensorReading.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible(distortionButton);
        distortionButton.setButtonText(TRANS("Distort Off"));
        distortionButton.addListener(this);
        distortionButton.setBounds(200, 180, 400, 104);
        distortionButton.setColour(TextButton::buttonColourId, Colour(192, 57, 43));
        
        addAndMakeVisible(distortionKnob);
        distortionKnob.setRange (1, 20, 1);
        distortionKnob.setValue (1);
        distortionKnob.setSliderStyle (Slider::RotaryHorizontalDrag);
        distortionKnob.setBounds(600, 180, 100, 100);
        distortionKnob.setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
        distortionKnob.addListener(this);
        
        // Phaser
        addAndMakeVisible(phaserButton);
        phaserButton.setButtonText(TRANS("Phaser Off"));
        phaserButton.addListener(this);
        phaserButton.setBounds(60, 300, 400, 104);
        phaserButton.setColour(TextButton::buttonColourId, Colour(192, 57, 43));
        
        addAndMakeVisible(phaserDepthKnob);
        phaserDepthKnob.setRange(0,10,1);
        phaserDepthKnob.setValue(1);
        phaserDepthKnob.setSliderStyle(Slider::RotaryHorizontalDrag);
        phaserDepthKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
        phaserDepthKnob.setBounds(460, 310, 100, 100);
        phaserDepthKnob.addListener(this);
        
        addAndMakeVisible(phaserRateKnob);
        phaserRateKnob.setRange(0,10,1);
        phaserRateKnob.setValue(1);
        phaserRateKnob.setSliderStyle(Slider::RotaryHorizontalDrag);
        phaserRateKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
        phaserRateKnob.setBounds(560, 310, 100, 100);
        phaserRateKnob.addListener(this);
        
        addAndMakeVisible(phaserWet);
        phaserWet.setRange(0, 100, 5);
        phaserWet.setValue(100);
        phaserWet.setSliderStyle(Slider::LinearBar);
        phaserWet.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
        phaserWet.setBounds(660, 310, 100, 50);
        phaserWet.addListener(this);
        
        setSize (800, 800);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    
    void sliderValueChanged(Slider *slider) override
    {
        if (slider == &freqSlider) {
            frequency = (float)freqSlider.getValue();
            phaseAngleChange = frequency * Ts * 2 * float_Pi;
        }
        if (slider == &distortionKnob) {
            distortionAlpha = (float)distortionKnob.getValue();
        }
        if (slider == &phaserDepthKnob) {
            phaserDepth = (float)phaserDepthKnob.getValue();
        }
        if (slider == &phaserRateKnob) {
            phaserRate = (float)phaserRateKnob.getValue();
        }
    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        if(buttonThatWasClicked == &stateButton) {
            switch(state)
            {
                case Instrument:
                    state = FXController;
                    stateButton.setButtonText("Switch to Instrument");
                    break;
                case FXController:
                    state = Instrument;
                    stateButton.setButtonText("Switch to FX Controller");
                    break;
            }
        }
        
        if (buttonThatWasClicked == &sineWaveButton)
        {
            currentWave = waves[0];
            distortionButton.setEnabled(true);
        }
        else if (buttonThatWasClicked == &squareWaveButton)
        {
            currentWave = waves[1];
            distortionButton.setEnabled(false);
        }
        else if (buttonThatWasClicked == &distortionButton)
        {
            enableDistortion = !enableDistortion;
            if(enableDistortion) {
                distortionButton.setColour(TextButton::buttonColourId, Colour(46, 204, 113));
                distortionButton.setButtonText(TRANS("Distort On"));
            } else {
                distortionButton.setColour(TextButton::buttonColourId, Colour(192, 57, 43));
                distortionButton.setButtonText(TRANS("Distort Off"));
            }
        }
        else if (buttonThatWasClicked == &enabledWaveButton)
        {
            enableWaves = !enableWaves;
            if (enableWaves) {
                enabledWaveButton.setColour(TextButton::buttonColourId, Colour(46, 204, 113));
                enabledWaveButton.setButtonText(TRANS("Wave Synthesis ON"));
            } else {
                enabledWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
                enabledWaveButton.setButtonText(TRANS("Wave Synthesis OFF"));
            }
        }
        else if (buttonThatWasClicked == &phaserButton)
        {
            enablePhaser = !enablePhaser;
            if(enablePhaser) {
                phaserButton.setColour(TextButton::buttonColourId, Colour(46, 204, 113));
                phaserButton.setButtonText(TRANS("Phaser On"));
            } else {
                phaserButton.setColour(TextButton::buttonColourId, Colour(192, 57, 43));
                phaserButton.setButtonText(TRANS("Phaser Off"));
            }
        }
    }
    
    enum AppState
    {
        Instrument,
        FXController
    };
    
    int sign(double value) { return (value >= 0.0) ? 1 : -1; }
    
    void prepareToPlay (int samplesPerBlockExpected, double thisSampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        
        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.
        
        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        
        String message;
        message << "Preparing to play audio..." + currentWave << newLine;
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << newLine;
        message << " sampleRate = " << thisSampleRate;
        Logger::getCurrentLogger()->writeToLog (message);
        
        amplitude = 0.5;
        frequency = 220;
        phaseAngle = 0.0;
        time = 0.0;
        deltaTime = 1/thisSampleRate;
        sampleRate = thisSampleRate;
        Ts = 1/sampleRate;
        
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if (enableWaves) {
            amplitude = 0.0;
        } else {
            amplitude = 0.5;
        }
        
        if (state == Instrument)
        {
            float *monoBuffer = new float[bufferToFill.numSamples];
        
            if (time >= std::numeric_limits<float>::max()) {
                time = 0.0;
            }
        
            if (currentWave == "sine") {
                // generate sin wave in mono
                for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                    phaseAngle += phaseAngleChange;
                    if (phaseAngle > (2 * float_Pi)) {
                        phaseAngle -= (2 * float_Pi);
                    }
                    float value = amplitude * sin(phaseAngle);
                
                    if(enableDistortion) {
                        value = distort(value);
                    }
                    if(enablePhaser) {
                        value = phaser(value);
                    }
                    monoBuffer[sample] = value;
                    time += deltaTime;
                }
            
            } else if (currentWave == "square") {
                // generate square wave in mono
                for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                    phaseAngle += phaseAngleChange;
                    if (phaseAngle > (2 * float_Pi)) {
                        phaseAngle -= (2 * float_Pi);
                    }
                    float value = amplitude * sign(sin(phaseAngle));
                
                    if(enablePhaser) {
                        value = phaser(value);
                    }
                    monoBuffer[sample] = value;
                    time += deltaTime;
                }
            }

        
            // iterate over all available output channels
            for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
            // Get a pointer to the start sample in the buffer for this audio output channel
                float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
            
                for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                    buffer[sample] = monoBuffer[sample];
                }
            }
        }
        else if (state == FXController) {
            AudioIODevice* device = deviceManager.getCurrentAudioDevice();
            const BigInteger activeInputChannels = device->getActiveInputChannels();
            const BigInteger activeOutputChannels = device->getActiveOutputChannels();
            const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;
            
            const float level = amplitude;
            
            for (int channel = 0; channel < maxOutputChannels; ++channel)
            {
                const float* inBuffer = bufferToFill.buffer->getReadPointer (1, bufferToFill.startSample);
                float* outBuffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
                        
                for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
                    outBuffer[sample] = distort(inBuffer[sample]) * level;
            }
        }
    }
    
    void releaseResources() override
    {
        
        Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }

    float distort(float value) {
        return 2/float_Pi * atan(value * distortionAlpha);
    }
    
    float phaser(float in) {
        return in;
    }
    
    //==============================================================================
    
//    void paint (Graphics& g) override
//    {
//        // (Our component is opaque, so we must completely fill the background with a solid colour)
//        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//
//
//        // You can add your drawing code here!
//    }
//
//    void resized() override
//    {
//
//    }
    

private:
    Slider freqSlider;
    Label sensor;
    AppState state;
    TextButton stateButton;
    TextButton sineWaveButton;
    TextButton squareWaveButton;
    TextButton enabledWaveButton;
    Label note;
    Label sensorReading;
    TextButton distortionButton;
    Slider distortionKnob;
    TextButton phaserButton;
    Slider phaserDepthKnob;
    Slider phaserRateKnob;
    Slider phaserWet;

    float amplitude;
    float frequency;
    float phaseAngle;
    float time;
    float deltaTime;
    float Ts;
    float sampleRate;
    float phaseAngleChange;
    String waves[3];
    String currentWave;
    bool enableWaves;
    
    // Phaser Stuff
    
    // Feed Forward Delay Buffer
    float ff[2] = {0, 0};
    // Feed Back Delay Buffer
    float fb[2] = {0, 0};
    
    float fbOut = 0.0;
    
    bool enableDistortion;
    int distortionAlpha;
    
    bool enablePhaser;
    float phaserRate;
    float phaserDepth;
    float wetPercent;
    
    void oscMessageReceived (const OSCMessage& message) override {
        if (message.size() == 1 && message[0].isFloat32()) {
            // Message logic here
            float value = message[0].getFloat32();
            float noteValue = round((220 * (pow(1.059463, value))) * 10) / 10;
            freqSlider.setValue(noteValue);
        }
    }
    
    void showConnectionErrorMessage (const String& messageText)
    {
        AlertWindow::showMessageBoxAsync (
                                          AlertWindow::WarningIcon,
                                          "Connection error",
                                          messageText,
                                          "OK");
    }
    
    void labelTextChanged(Label* labelThatChanged) override
    {
        
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
