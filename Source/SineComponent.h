/*
  ==============================================================================

    SineComponent.h
    Created: 27 Jan 2018 2:43:41am
    Author:  Nick Chouard

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SineComponent    : public Component
{
public:
    SineComponent();
    ~SineComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineComponent)
};
