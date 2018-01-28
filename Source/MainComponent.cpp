
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
        
        addAndMakeVisible(sensor);
        sensor.setText(String(frequency), dontSendNotification);
        sensor.setFont( (Font (20.00f, Font::plain).withTypefaceStyle("Bold")));
        
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
        
        waves[0] = { "sine"};
        waves[1] = {"square"};
        currentWave = waves[0];
        enableDistortion = false;
        distortionAlpha = 1;
        
        addAndMakeVisible (sineWaveButton);
        sineWaveButton.setButtonText (TRANS("Sine Wave"));
        sineWaveButton.addListener (this);
        sineWaveButton.setBounds (0, 30, 100, 100);
        
        addAndMakeVisible (squareWaveButton);
        squareWaveButton.setButtonText (TRANS("Square Wave"));
        squareWaveButton.addListener (this);
        squareWaveButton.setBounds (130, 30, 100, 100);
        
        addAndMakeVisible (sensorReading);
        sensorReading.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        sensorReading.setJustificationType (Justification::centredLeft);
        sensorReading.setEditable (false, false, false);
        sensorReading.setColour (TextEditor::textColourId, Colours::black);
        sensorReading.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible(distortionButton);
        distortionButton.setButtonText(TRANS("Distort Off"));
        distortionButton.addListener(this);
        distortionButton.setBounds (200, 180, 100, 100);
        distortionButton.setColour(TextButton::buttonColourId, Colour(192, 57, 43));
        
        addAndMakeVisible(distortionKnob);
        distortionKnob.setRange (1, 20, 1);
        distortionKnob.setValue (1);
        distortionKnob.setSliderStyle (Slider::RotaryHorizontalDrag);
        distortionKnob.setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
        distortionKnob.setBounds(700, 190, 100, 100);
        distortionKnob.addListener(this);
        
        setSize (1100, 800);
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
    Label note;
    Label sensorReading;
    TextButton distortionButton;
    Slider distortionKnob;

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
    
    bool enableDistortion;
    int distortionAlpha;
    
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
