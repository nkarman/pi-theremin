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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "HarmonizerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
HarmonizerComponent::HarmonizerComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (thirdButton = new TextButton ("third"));
    thirdButton->setButtonText (TRANS("third"));
    thirdButton->addListener (this);

    addAndMakeVisible (fifthButton = new TextButton ("fifth"));
    fifthButton->setButtonText (TRANS("fifth"));
    fifthButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HarmonizerComponent::~HarmonizerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    thirdButton = nullptr;
    fifthButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HarmonizerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HarmonizerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    thirdButton->setBounds (140, 300, 71, 24);
    fifthButton->setBounds (220, 300, 71, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void HarmonizerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == thirdButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == fifthButton)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HarmonizerComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="3rd" id="17a6b08c1d754968" memberName="textButton" virtualName=""
              explicitFocusOrder="0" pos="41 200 71 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="cdc6bda6f4f22c1e" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="136 200 72 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
