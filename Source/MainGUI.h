/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "HarmonizerComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainGUI  : public Component,
                 public Button::Listener
{
public:
    //==============================================================================
    MainGUI ();
    ~MainGUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* sinWave_png;
    static const int sinWave_pngSize;
    static const char* squareWave_png;
    static const int squareWave_pngSize;
    
    String waves[3];
    static String currentWave;
    
    HarmonizerComponent harmonizer;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> sineWaveButton;
    ScopedPointer<TextButton> squareWaveButton;
    ScopedPointer<TextButton> triangleWaveButton;
    ScopedPointer<Label> note;
    ScopedPointer<Label> sensorReading;
    ScopedPointer<TextButton> plusOctave;
    ScopedPointer<Label> octave;
    ScopedPointer<TextButton> minusOctave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainGUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
