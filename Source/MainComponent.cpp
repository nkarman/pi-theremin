/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                               private OSCReceiver,
                               private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>,
                               public Slider::Listener,
                               public TextButton::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        // specify a UDP port to connect to with OSCReceiver
        /*
        if (!connect(6969))
            std::cout << "Tried to Connect" << std::endl;
            showConnectionErrorMessage("Error: could not connect to port 6969");
        */
        connect(6969);
        // listen to OSC messages at this address at port 6969
        addListener(this, "/theremin/midi");
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        // Volume
        addAndMakeVisible(volumeSlider);
        volumeSlider.setRange(-96, 6);
        volumeSlider.setTextValueSuffix(" db");
        volumeSlider.setValue(-6);
        volumeSlider.addListener(this);
        //volumeSlider.setSkewFactorFromMidPoint(0.5);
        
        volumeLabel.setText("Volume", dontSendNotification);
        volumeLabel.attachToComponent(&volumeSlider, true);
        
        // phase slider
        addAndMakeVisible(phaseSlider);
        phaseSlider.setRange(0.0, 1.0);
        phaseSlider.setTextValueSuffix(" ~");
        phaseSlider.setValue(0.0);
        phaseSlider.addListener(this);
        
        phaseLabel.setText("Phase", dontSendNotification);
        phaseLabel.attachToComponent(&phaseSlider, true);
        
        // freq slider
        addAndMakeVisible(freqSlider);
        freqSlider.setRange(10, 22000);
        freqSlider.setTextValueSuffix(" Hz");
        freqSlider.setValue(440.0);
        freqSlider.addListener(this);
        freqSlider.setSkewFactorFromMidPoint(500);
        
        freqLabel.setText("Freq", dontSendNotification);
        freqLabel.attachToComponent(&freqSlider, true);
        
        // mute button
        addAndMakeVisible(m_muteButton);
        m_muteButton.setButtonText("Mute");
        m_muteButton.addListener(this);
        m_muteButton.setEnabled(true);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void buttonClicked(Button* button) override
    {
        if (button == &m_muteButton) {
            m_mute = !m_mute;
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
        message << "Preparing to play audio..." << newLine;
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
        
        // Your audio-processing code goes here!
        
        // For more details, see the help for AudioProcessor::getNextAudioBlock()
        
        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        //bufferToFill.clearActiveBufferRegion();
        
        if (sinTime >= std::numeric_limits<float>::max()) {
            sinTime = 0.0;
        }
        
        float *monoBuffer = new float[bufferToFill.numSamples];
        
        
        // generate sin wave in mono
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
            float value = sinAmplitude * sign(sin(2 * double_Pi * sinFrequency * sinTime + sinPhase));
            
            monoBuffer[sample] = value;
            sinTime += sinDeltaTime;
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
        const int sliderLeft = 50;
        volumeSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
        phaseSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
        freqSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
        m_muteButton.setBounds(10, 110, getWidth() - 20, 20);
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
    
    TextButton m_muteButton;
    bool m_mute;
    
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
