
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
        

        addAndMakeVisible(freqSlider);
        freqSlider.setRange(10, 22000);
        freqSlider.setTextValueSuffix(" Hz");
        freqSlider.setValue(220.0);
        freqSlider.addListener(this);
        freqSlider.setSkewFactorFromMidPoint(500);
        
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
        float *monoBuffer = new float[bufferToFill.numSamples];
        
        if (time >= std::numeric_limits<float>::max()) {
            time = 0.0;
        }
        
        if (MainGUI.currentWave == "sine") {
            // generate sin wave in mono
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
                phaseAngle += phaseAngleChange;
                if (phaseAngle > (2 * float_Pi)) {
                    phaseAngle -= (2 * float_Pi);
                }
                float value = amplitude * sin(phaseAngle);
                monoBuffer[sample] = value;
                time += deltaTime;
            }
            
        } else if (MainGUI.currentWave == "square") {
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
    Slider freqSlider;
    MainGUI MainGUI;

    float amplitude;
    float frequency;
    float phaseAngle;
    float time;
    float deltaTime;
    float Ts;
    float sampleRate;
    float phaseAngleChange;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
