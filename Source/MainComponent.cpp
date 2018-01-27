
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainGUI.h"



class MainContentComponent   : public AudioAppComponent,
                               private OSCReceiver,
                               private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>,
                               public Slider::Listener,
                               public TextButton::Listener
                                
{
public:
    MainContentComponent()
    {
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        // specify a UDP port to connect to with OSCReceiver
        connect(6969);
        // listen to OSC messages at this address at port 6969
        addListener(this, "/theremin/midi");
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        addAndMakeVisible (sineWaveButton = new TextButton ("sineWaveButton"));
        sineWaveButton->addListener (this);
        
        addAndMakeVisible (squareWaveButton = new TextButton ("squareWaveButton"));
        squareWaveButton->addListener (this);
        
        addAndMakeVisible (triangleWaveButton = new TextButton ("triangleWaveButton"));
        triangleWaveButton->addListener (this);
        
        addAndMakeVisible (note = new Label ("note",
                                             TRANS("Notes")));
        note->setFont (Font (23.50f, Font::plain).withTypefaceStyle ("Regular"));
        note->setJustificationType (Justification::centred);
        note->setEditable (false, false, false);
        note->setColour (Label::backgroundColourId, Colours::blueviolet);
        note->setColour (TextEditor::textColourId, Colours::black);
        note->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible (sensorReading = new Label ("sensorReading",
                                                      TRANS("sensor reading\n")));
        sensorReading->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        sensorReading->setJustificationType (Justification::centredLeft);
        sensorReading->setEditable (false, false, false);
        sensorReading->setColour (TextEditor::textColourId, Colours::black);
        sensorReading->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible (plusOctave = new TextButton ("plusOctave"));
        plusOctave->setButtonText (TRANS("+"));
        plusOctave->addListener (this);
        
        addAndMakeVisible (octave = new Label ("octave",
                                               TRANS("octave #")));
        octave->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        octave->setJustificationType (Justification::centredLeft);
        octave->setEditable (false, false, false);
        octave->setColour (TextEditor::textColourId, Colours::black);
        octave->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        addAndMakeVisible (minusOctave = new TextButton ("minusOctave"));
        minusOctave->setButtonText (TRANS("-"));
        minusOctave->addListener (this);
        
        setSize (800, 600);
        
        // Volume
        //        addAndMakeVisible(volumeSlider);
        //        volumeSlider.setRange(-96, 6);
        //        volumeSlider.setTextValueSuffix(" db");
        //        volumeSlider.setValue(-6);
        //        volumeSlider.addListener(this);
        //        //volumeSlider.setSkewFactorFromMidPoint(0.5);
        //
        //        volumeLabel.setText("Volume", dontSendNotification);
        //        volumeLabel.attachToComponent(&volumeSlider, true);
        //
        //        // phase slider
        //        addAndMakeVisible(phaseSlider);
        //        phaseSlider.setRange(0.0, 1.0);
        //        phaseSlider.setTextValueSuffix(" ~");
        //        phaseSlider.setValue(0.0);
        //        phaseSlider.addListener(this);
        //
        //        phaseLabel.setText("Phase", dontSendNotification);
        //        phaseLabel.attachToComponent(&phaseSlider, true);
        //
        //        // freq slider
        //        addAndMakeVisible(freqSlider);
        //        freqSlider.setRange(10, 22000);
        //        freqSlider.setTextValueSuffix(" Hz");
        //        freqSlider.setValue(440.0);
        //        freqSlider.addListener(this);
        //        freqSlider.setSkewFactorFromMidPoint(500);
        //
        //        freqLabel.setText("Freq", dontSendNotification);
        //        freqLabel.attachToComponent(&freqSlider, true);
        //
        //        // mute button
        //        addAndMakeVisible(m_muteButton);
        //        m_muteButton.setButtonText("Mute");
        //        m_muteButton.addListener(this);
        //        m_muteButton.setEnabled(true);
        
    }

    ~MainContentComponent()
    {
        sineWaveButton = nullptr;
        squareWaveButton = nullptr;
        triangleWaveButton = nullptr;
        note = nullptr;
        sensorReading = nullptr;
        plusOctave = nullptr;
        octave = nullptr;
        minusOctave = nullptr;
        shutdownAudio();
    }

    //==============================================================================
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {

        
        if (buttonThatWasClicked == sineWaveButton)
        {
            currentWave = waves[0];
            std::cout << currentWave << std::endl;
        }
        else if (buttonThatWasClicked == squareWaveButton)
        {
            currentWave = waves[1];
            std::cout << currentWave << std::endl;
        }
        else if (buttonThatWasClicked == triangleWaveButton)
        {
            currentWave = waves[2];
            std::cout << currentWave << std::endl;
        }
        else if (buttonThatWasClicked == plusOctave)
        {

        }
        else if (buttonThatWasClicked == minusOctave)
        {

        }
        
    }
    
    void sliderValueChanged(Slider *slider) override
    {
        if (slider == &volumeSlider) {
            sinAmplitude = pow(10, ((float)volumeSlider.getValue() / 20.0));
        }
        
        if (slider == &freqSlider) {
            sinFrequency = (float)freqSlider.getValue();
        }
        
        if (slider == &phaseSlider) {
            sinPhase = (float)phaseSlider.getValue();
        }
    }
    
    int sign(double value) { return (value >= 0.0) ? 1 : -1; }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        
        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.
        
        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        
        String message;
        message << "Preparing to play audio..." + currentWave << newLine;
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << newLine;
        message << " sampleRate = " << sampleRate;
        Logger::getCurrentLogger()->writeToLog (message);
        
        sinAmplitude = 0.5;
        sinFrequency = 440;
        sinPhase = 0.0;
        sinTime = 0.0;
        sinDeltaTime = 1/sampleRate;
        
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        float *monoBuffer = new float[bufferToFill.numSamples];
        
        if (sinTime >= std::numeric_limits<float>::max()) {
            sinTime = 0.0;
        }
        
        if (currentWave == "sine") {
            // generate sin wave in mono
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                float value = sinAmplitude * sin(2 * double_Pi * sinFrequency * sinTime + sinPhase);
                monoBuffer[sample] = value;
                sinTime += sinDeltaTime;
            }
            
        } else if (currentWave == "square") {
            // generate square wave in mono
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                float value = sinAmplitude * sign(sin(2 * double_Pi * sinFrequency * sinTime + sinPhase));
                
                monoBuffer[sample] = value;
                sinTime += sinDeltaTime;
            }
        } else if (currentWave == "triange") {
            
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
    
    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
        
        // For more details, see the help for AudioProcessor::releaseResources()
        
        Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
//        const int sliderLeft = 50;
//        volumeSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
//        phaseSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
//        freqSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
//        m_muteButton.setBounds(10, 110, getWidth() - 20, 20);
        
        sineWaveButton->setBounds (0, 0, (getWidth() / 3) - 10, 104);
        squareWaveButton->setBounds (getWidth() / 3 + 5, 0, (getWidth() / 3) - 10, 104);
        triangleWaveButton->setBounds (getWidth() / 1.5 + 10 , 0, (getWidth() / 3) - 10, 104);
        note->setBounds (16, 136, 88, 64);
        sensorReading->setBounds (8, 208, 150, 24);
        plusOctave->setBounds (88, 264, 31, 24);
        octave->setBounds (32, 264, 55, 24);
        minusOctave->setBounds (0, 264, 31, 24);
    }


private:
    //==============================================================================

    // Your private member variables go here...
    // GUI
    
    Slider volumeSlider;
    Slider freqSlider;
    Slider phaseSlider;
    Label volumeLabel;
    Label freqLabel;
    Label phaseLabel;
    MainGUI MainGUI;
    
    ScopedPointer<TextButton> sineWaveButton;
    ScopedPointer<TextButton> squareWaveButton;
    ScopedPointer<TextButton> triangleWaveButton;
    ScopedPointer<Label> note;
    ScopedPointer<Label> sensorReading;
    ScopedPointer<TextButton> plusOctave;
    ScopedPointer<Label> octave;
    ScopedPointer<TextButton> minusOctave;
    
    TextButton m_muteButton;
    bool m_mute;
    String waves [3] = { "sine", "square", "triange" };
    String currentWave = waves[0];
    
    void oscMessageReceived (const OSCMessage& message) override {
        if (message.size() == 1 && message[0].isInt32()) {
            // Message logic here
            int value = message[0].getInt32();
            std::cout << value << std::endl;
            double noteValue = round((440 * (pow(1.059463, value))) * 10) / 10;
            
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

    
    Random random;
    
    float sinAmplitude;
    float sinFrequency;
    float sinPhase;
    float sinTime;
    float sinDeltaTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
