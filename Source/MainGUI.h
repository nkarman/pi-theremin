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

#include "../JuceLibraryCode/JuceHeader.h"

class MainGUI  : public Component,
                 public Button::Listener
{
public:
    MainGUI ();
    ~MainGUI();

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

private:
    ScopedPointer<TextButton> sineWaveButton;
    ScopedPointer<TextButton> squareWaveButton;
    ScopedPointer<Label> note;
    ScopedPointer<Label> sensorReading;
    ScopedPointer<TextButton> plusOctave;
    ScopedPointer<Label> octave;
    ScopedPointer<TextButton> minusOctave;
    LookAndFeel_V4 otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainGUI)
};

