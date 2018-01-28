
#include "../JuceLibraryCode/JuceHeader.h"

#define phaserlfoshape 4.0
#define lfoskipsamples 20

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
        enableReverb = true;
        enablePhaser = false;
        reverbUnit.setSampleRate(sampleRate);
        params.freezeMode = 0.4;
        params.width = 0.5;
        
        addAndMakeVisible(sensor);
        sensor.setText(String(frequency), dontSendNotification);
        sensor.setFont( (Font (20.00f, Font::plain).withTypefaceStyle("Bold")));
        
        addAndMakeVisible(stateButton);
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
        sineWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
        
        addAndMakeVisible (squareWaveButton);
        squareWaveButton.setButtonText (TRANS("Square Wave"));
        squareWaveButton.addListener (this);
        squareWaveButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
        
        addAndMakeVisible(enabledWaveButton);
        enabledWaveButton.setButtonText (TRANS("Wave Synthesis ON"));
        enabledWaveButton.addListener(this);
        enabledWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
        
        addAndMakeVisible (sensorReading);
        sensorReading.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        sensorReading.setJustificationType (Justification::centredLeft);
        sensorReading.setEditable (false, false, false);
        sensorReading.setColour (TextEditor::textColourId, Colours::black);
        sensorReading.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible(distortionButton);
        distortionButton.setButtonText(TRANS("Distort Off"));
        distortionButton.addListener(this);
        distortionButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
        
        addAndMakeVisible(distortionKnob);
        distortionKnob.setRange (1, 20, 1);
        distortionKnob.setValue (1);
        distortionKnob.setSliderStyle (Slider::RotaryHorizontalDrag);
        distortionKnob.setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
        distortionKnob.addListener(this);
        
        addAndMakeVisible(reverbButton);
        reverbButton.setButtonText(TRANS("Reverb Off"));
        reverbButton.addListener(this);
        reverbButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
        
        addAndMakeVisible(reverbRoomSizeKnob);
        reverbRoomSizeKnob.setRange(0,1,0.1);
        reverbRoomSizeKnob.setValue(1);
        reverbRoomSizeKnob.setSliderStyle(Slider::RotaryHorizontalDrag);
        reverbRoomSizeKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
        reverbRoomSizeKnob.addListener(this);
        
        addAndMakeVisible(phaserWet);
        phaserWet.setRange(0, 100, 5);
        phaserWet.setValue(100);
        phaserWet.setSliderStyle(Slider::LinearBar);
        phaserWet.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
        phaserWet.addListener(this);
        
        enabledWaveButton.setBounds(375, 5, 50, 20);
        sensor.setBounds(getWidth() / 2, getHeight() / 3 - 10, 100, 50);
        squareWaveButton.setBounds(420, 40, 360, 100);
        sineWaveButton.setBounds (20, 40, 360, 100);
        distortionButton.setBounds(50, 180, 400, 104);
        distortionKnob.setBounds(450, 180, 100, 100);
        reverbButton.setBounds(50, 300, 400, 104);
        reverbRoomSizeKnob.setBounds(450, 310, 100, 100);
        //phaserWet.setBounds(650, 310, 100, 50);
        stateButton.setBounds(350, 700, 100, 100);
        reverbButton.setEnabled(false);
        
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
        if (slider == &reverbRoomSizeKnob) {
            params.roomSize = (float)reverbRoomSizeKnob.getValue();
            reverbUnit.setParameters(params);
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
                    reverbButton.setEnabled(true);
                    distortionButton.setEnabled(false);
                    break;
                case FXController:
                    state = Instrument;
                    stateButton.setButtonText("Switch to FX Controller");
                    reverbButton.setEnabled(false);
                    distortionButton.setEnabled(true);
                    break;
            }
        }
        
        else if (buttonThatWasClicked == &sineWaveButton)
        {
            currentWave = waves[0];
            distortionButton.setEnabled(true);
            sineWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
            squareWaveButton.setColour(TextButton::buttonColourId, Colour(0x00000000));
        }
        else if (buttonThatWasClicked == &squareWaveButton)
        {
            currentWave = waves[1];
            distortionButton.setEnabled(false);
            squareWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
            sineWaveButton.setColour(TextButton::buttonColourId, Colour(0x00000000));
        }
        else if (buttonThatWasClicked == &distortionButton)
        {
            enableDistortion = !enableDistortion;
            if(enableDistortion) {
                distortionButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
                distortionButton.setButtonText(TRANS("Distort On"));
            } else {
                distortionButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
                distortionButton.setButtonText(TRANS("Distort Off"));
            }
        }
        else if (buttonThatWasClicked == &enabledWaveButton)
        {
            enableWaves = !enableWaves;
            if (enableWaves) {
                enabledWaveButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
                enabledWaveButton.setButtonText(TRANS("Wave Synthesis ON"));
            } else {
                enabledWaveButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
                enabledWaveButton.setButtonText(TRANS("Wave Synthesis OFF"));
            }
        }
        else if (buttonThatWasClicked == &reverbButton)
        {
            enableReverb = !enableReverb;
            if(enableReverb) {
                reverbButton.setColour(TextButton::buttonColourId, Colour(72, 156, 229));
                reverbButton.setButtonText(TRANS("Reverb On"));
                reverbRoomSizeKnob.setEnabled(true);
            } else {
                reverbButton.setColour(TextButton::buttonColourId, Colour (0x00000000));
                reverbButton.setButtonText(TRANS("Reverb Off"));
                reverbRoomSizeKnob.setEnabled(false);
                // Clears old Phaser values
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
            amplitude = 0.5;
        } else {
            amplitude = 0.0;
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
                    monoBuffer[sample] = value;
                    if(enablePhaser) {
                        phaser(monoBuffer, bufferToFill.numSamples);
                    }
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
                    if(enablePhaser) {
                        phaser(monoBuffer, bufferToFill.numSamples);
                    }
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
                    outBuffer[sample] = inBuffer[sample] * level;
                int numSamples = bufferToFill.numSamples;
                if(enableReverb) reverbUnit.processMono(outBuffer, numSamples);
            }
        }
    }
    
    void releaseResources() override
    {
        Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }
    
    void process(float* buffer, int numSamples) {
        // apply processing
        if(enableDistortion) {
            for (int sample = 0; sample < sizeof(buffer); ++sample)
                buffer[sample] = distort(buffer[sample]);
        }
        if(enableReverb) {
            reverb(buffer, numSamples);
        }
    }
    
    float distort(float value) {
        return 2/float_Pi * atan(value * distortionAlpha);
    }
    
    void phaser(float* buffer, int numSamples) {
        float m, tmp, out;
        
        for (int sample = 0; sample < numSamples; ++sample) {
            int j;
            float in = buffer[sample];
            m = in + fbOut * 5 / 100;
            if (((skipCount++) % lfoskipsamples) == 0) {
                gain = (1 + cos(skipCount * lfoSkip + phase)) / 2;
                gain =(exp(gain * phaserlfoshape) - 1) / (exp(phaserlfoshape)-1);
                gain = 1 - gain / 255 * phaserDepth;
            }
            for (j = 0; j < stages; j++) {
                tmp = old[j];
                old[j] = gain * tmp + m;
                m = tmp - gain * old[j];
            }
            fbOut = m;
            out = (m * wetPercent + in * (255 - wetPercent)) / 255;
            if (out < -1.0) out = -1.0;
            if (out > 1.0) out = 1.0;
            buffer[sample] = out;
        }
    }
    
    void reverb(float* buffer, int numSamples) {
        reverbUnit.processMono(buffer, numSamples);
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
    TextButton reverbButton;
    Slider phaserWet;
    Slider reverbRoomSizeKnob;
    Reverb reverbUnit;
    Reverb::Parameters params;
    
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
    bool enableReverb;
    
    int stages = 10;
    float fbOut = 0.0;
    unsigned long skipCount = 0;
    float gain = 0.0;
    float lfoSkip = 500 * 2 * M_PI / 44100;
    float startPhase = 30;
    float phase = startPhase * M_PI / 180;
    float old[24];
    
    // Phaser Stuff
    
    // Feed Forward Delay Buffer
    float ff[2] = {0, 0};
    // Feed Back Delay Buffer
    float fb[2] = {0, 0};
    
    bool enableDistortion;
    int distortionAlpha;
    
    bool enablePhaser;
    float phaserRate;
    float phaserDepth;
    float wetPercent;
    
    void oscMessageReceived (const OSCMessage& message) override {
        if (message.size() == 1 && message[0].isFloat32()) {
            if(state == Instrument) {
                // Message logic here
                float value = message[0].getFloat32();
                float noteValue = round(220 * pow(2, (value/12.0)) * 10) / 10;
                freqSlider.setValue(noteValue);
            }
            else if(state == FXController) {
                if(enableDistortion) {
                    float value = message[0].getFloat32();
                    float knobValue = round(value * 2) - 4;
                    distortionKnob.setValue(knobValue);
                }
                if(enableReverb) {
                    float value = message[0].getFloat32();
                    float roomSize = (value - 2) / 10;
                    reverbRoomSizeKnob.setValue(roomSize);
                }
            }
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
