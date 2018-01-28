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

#include "Filters.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Filters::Filters ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (Filter = new Label ("Filter",
                                           TRANS("Filters\n")));
    Filter->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Filter->setJustificationType (Justification::centred);
    Filter->setEditable (false, false, false);
    Filter->setColour (TextEditor::textColourId, Colours::black);
    Filter->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (HPFKnob = new Slider ("HPFKnob"));
    HPFKnob->setRange (20, 1000, 20);
    HPFKnob->setSliderStyle (Slider::RotaryHorizontalDrag);
    HPFKnob->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    HPFKnob->addListener (this);

    addAndMakeVisible (LPFKnob = new Slider ("HPFKnob"));
    LPFKnob->setRange (20, 1000, 20);
    LPFKnob->setSliderStyle (Slider::RotaryHorizontalDrag);
    LPFKnob->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    LPFKnob->addListener (this);

    addAndMakeVisible (HPFLabel = new Label ("HPFLabel",
                                             TRANS("HPF")));
    HPFLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    HPFLabel->setJustificationType (Justification::centred);
    HPFLabel->setEditable (false, false, false);
    HPFLabel->setColour (TextEditor::textColourId, Colours::black);
    HPFLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (LPFLabel = new Label ("LPFLabel",
                                             TRANS("LPF")));
    LPFLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    LPFLabel->setJustificationType (Justification::centred);
    LPFLabel->setEditable (false, false, false);
    LPFLabel->setColour (TextEditor::textColourId, Colours::black);
    LPFLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Filters::~Filters()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    Filter = nullptr;
    HPFKnob = nullptr;
    LPFKnob = nullptr;
    HPFLabel = nullptr;
    LPFLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Filters::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        int x = 92, y = 92, width = 284, height = 188;
        Colour fillColour = Colour (0xff8943a7);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Filters::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    Filter->setBounds (160, 93, 150, 24);
    HPFKnob->setBounds (112, 152, 95, 96);
    LPFKnob->setBounds (248, 152, 95, 96);
    HPFLabel->setBounds (140, 128, 39, 24);
    LPFLabel->setBounds (275, 128, 39, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Filters::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == HPFKnob)
    {
        //[UserSliderCode_HPFKnob] -- add your slider handling code here..
        //[/UserSliderCode_HPFKnob]
    }
    else if (sliderThatWasMoved == LPFKnob)
    {
        //[UserSliderCode_LPFKnob] -- add your slider handling code here..
        //[/UserSliderCode_LPFKnob]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Filters" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="92 92 284 188" fill="solid: ff8943a7" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="Filter" id="36c473a1d5e952a0" memberName="Filter" virtualName=""
         explicitFocusOrder="0" pos="160 93 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filters&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="36"/>
  <SLIDER name="HPFKnob" id="48edb7e44d095a41" memberName="HPFKnob" virtualName=""
          explicitFocusOrder="0" pos="112 152 95 96" min="20" max="1000"
          int="20" style="RotaryHorizontalDrag" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="HPFKnob" id="c0c13a965b7b8a16" memberName="LPFKnob" virtualName=""
          explicitFocusOrder="0" pos="248 152 95 96" min="20" max="1000"
          int="20" style="RotaryHorizontalDrag" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="HPFLabel" id="d8837a8de14c3dc5" memberName="HPFLabel" virtualName=""
         explicitFocusOrder="0" pos="140 128 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="HPF" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="LPFLabel" id="a4aee3a3f9b7e20" memberName="LPFLabel" virtualName=""
         explicitFocusOrder="0" pos="275 128 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="LPF" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
