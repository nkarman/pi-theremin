
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainGUI.h"



class MainContentComponent   : public AudioAppComponent,
                               private OSCReceiver,
                               private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>,
                               public Slider::Listener
                               //public TextButton::Listener,

{
public:
    MainContentComponent()
    {
        addAndMakeVisible(MainGUI);
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        // specify a UDP port to connect to with OSCReceiver
        connect(6969);
        // listen to OSC messages at this address at port 6969
        addListener(this, "/theremin/midi");
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        
        setSize (1100, 800);
        
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
                // freq slider
                addAndMakeVisible(freqSlider);
                freqSlider.setRange(10, 22000);
                freqSlider.setTextValueSuffix(" Hz");
                freqSlider.setValue(440.0);
                freqSlider.addListener(this);
                freqSlider.setSkewFactorFromMidPoint(500);
        //
                freqLabel.setText("Freq", dontSendNotification);
                freqLabel.attachToComponent(&freqSlider, true);
        //
        //        // mute button
        //        addAndMakeVisible(m_muteButton);
        //        m_muteButton.setButtonText("Mute");
        //        m_muteButton.addListener(this);
        //        m_muteButton.setEnabled(true);
        
    }

    ~MainContentComponent()
    {
        note = nullptr;
        sensorReading = nullptr;
        plusOctave = nullptr;
        octave = nullptr;
        minusOctave = nullptr;
        shutdownAudio();
    }

    //==============================================================================
    
    void sliderValueChanged(Slider *slider) override
    {
        if (slider == &volumeSlider) {
            sinAmplitude = pow(10, ((float)volumeSlider.getValue() / 20.0));
        }
        
        if (slider == &freqSlider) {
            frequency = (float)freqSlider.getValue();
            phaseAngleChange = frequency * Ts * 2 * float_Pi;
            
        }
        
        if (slider == &phaseSlider) {
            phaseAngle = (float)phaseSlider.getValue();
        }
    }
    
    int sign(double value) { return (value >= 0.0) ? 1 : -1; }
    
    void prepareToPlay (int samplesPerBlockExpected, double thisSampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        
        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.
        
        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        
        String message;
        message << "Preparing to play audio..." + MainGUI.currentWave << newLine;
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << newLine;
        message << " sampleRate = " << thisSampleRate;
        Logger::getCurrentLogger()->writeToLog (message);
        
        sinAmplitude = 0.5;
        frequency = 220;
        phaseAngle = 0.0;
        sinTime = 0.0;
        sinDeltaTime = 1/thisSampleRate;
        sampleRate = thisSampleRate;
        Ts = 1/sampleRate;
        responseTime = 0.25;
        prevFrequency = frequency;
        prevFreqSmooth = 1.0;
        
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        float *monoBuffer = new float[bufferToFill.numSamples];
        
        if (sinTime >= std::numeric_limits<float>::max()) {
            sinTime = 0.0;
        }
        
        if (MainGUI.currentWave == "sine") {
            // generate sin wave in mono
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                phaseAngle += phaseAngleChange;
                if (phaseAngle > (2 * float_Pi)) {
                    phaseAngle -= (2 * float_Pi);
                }
                float value = sinAmplitude * sin(phaseAngle);
                monoBuffer[sample] = value;
                sinTime += sinDeltaTime;
            }
            
        } else if (MainGUI.currentWave == "square") {
            // generate square wave in mono
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                phaseAngle += phaseAngleChange;
                if (phaseAngle > (2 * float_Pi)) {
                    phaseAngle -= (2 * float_Pi);
                }
                float value = sinAmplitude * sign(sin(phaseAngle));
                
                monoBuffer[sample] = value;
                sinTime += sinDeltaTime;
            }
        } else if (MainGUI.currentWave == "triange") {
            
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
    /*
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    

        // You can add your drawing code here!
    }
    
    void resized() override
    {
        freqSlider.setBounds(50, 250, getWidth() - 60, 20);
        sineWaveButton->setBounds (0, 10, (getWidth() / 3) - 10, 104);
        squareWaveButton->setBounds (getWidth() / 3 + 5, 10, (getWidth() / 3) - 10, 104);
        triangleWaveButton->setBounds (getWidth() / 1.5 + 10 , 10, (getWidth() / 3) - 10, 104);
        note->setBounds (16, 136, 88, 64);
        sensorReading->setBounds (8, 208, 150, 24);
        plusOctave->setBounds (88, 264, 31, 24);
        octave->setBounds (32, 264, 55, 24);
        minusOctave->setBounds (0, 264, 31, 24);
    }
     */

private:
    Slider volumeSlider;
    Slider freqSlider;
    Slider phaseSlider;
    Label volumeLabel;
    Label freqLabel;
    Label phaseLabel;
    MainGUI MainGUI;
    
    ScopedPointer<Label> note;
    ScopedPointer<Label> sensorReading;
    ScopedPointer<TextButton> plusOctave;
    ScopedPointer<Label> octave;
    ScopedPointer<TextButton> minusOctave;
    
    TextButton m_muteButton;
    bool m_mute;
    
    void oscMessageReceived (const OSCMessage& message) override {
        if (message.size() == 1 && message[0].isFloat32()) {
            // Message logic here
            float value = message[0].getFloat32();
            double noteValue = round((220 * (pow(1.059463, value))) * 10) / 10;
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
    float frequency;
    float prevFrequency;
    float freqSmooth;
    float phaseAngle;
    float sinTime;
    float sinDeltaTime;
    float Ts;
    float sampleRate;
    float responseTime;
    float prevFreqSmooth;
    float phaseAngleChange;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
