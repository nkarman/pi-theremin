
#include "MainGUI.h"

String MainGUI::currentWave;

MainGUI::MainGUI ()
{
    waves[0] = { "sine"};
    waves[1] = {"square"};
    currentWave = waves[0];
    
    addAndMakeVisible (sineWaveButton = new TextButton("sineWaveButton"));
    sineWaveButton->setButtonText (TRANS("Sine Wave"));
    sineWaveButton->addListener (this);
    sineWaveButton->setBounds (0, 30, (getWidth() / 2), 104);

    addAndMakeVisible (squareWaveButton = new TextButton ("squareWaveButton"));
    squareWaveButton->setButtonText (TRANS("Square Wave"));
    squareWaveButton->addListener (this);
    squareWaveButton->setBounds (130, 30, (getWidth() / 2), 104);

    addAndMakeVisible (sensorReading = new Label ("sensorReading",
                                                  TRANS("sensor reading\n")));
    sensorReading->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sensorReading->setJustificationType (Justification::centredLeft);
    sensorReading->setEditable (false, false, false);
    sensorReading->setColour (TextEditor::textColourId, Colours::black);
    sensorReading->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    setSize (800, 800);


}

MainGUI::~MainGUI()
{

    sineWaveButton = nullptr;
    squareWaveButton = nullptr;
    sensorReading = nullptr;

}

//==============================================================================
void MainGUI::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void MainGUI::resized()
{
    sineWaveButton->setBounds (0, 30, 100, 104);
    squareWaveButton->setBounds (130, 30, 100, 104);
    
}

void MainGUI::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == sineWaveButton)
    {
        currentWave = waves[0];
        
    }
    else if (buttonThatWasClicked == squareWaveButton)
    {
        currentWave = waves[1];

    }
}



//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainGUI" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="20 140 624 11" fill="solid: ff2aa547" hasStroke="0"/>
    <RECT pos="188 156 12 408" fill="solid: ff2a38a5" hasStroke="0"/>
    <RECT pos="460 156 12 412" fill="solid: ff2a38a5" hasStroke="0"/>
  </BACKGROUND>
  <TEXTBUTTON name="sineWaveButton" id="3e375b5f735d35ca" memberName="sineWaveButton"
              virtualName="" explicitFocusOrder="0" pos="16 16 160 104" buttonText="Sine Wave"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="squareWaveButton" id="c624252c29af3aa4" memberName="squareWaveButton"
              virtualName="" explicitFocusOrder="0" pos="240 16 176 104" buttonText="Square Wave"
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="triangleWaveButton" id="1c878e15cd0b9254" memberName="triangleWaveButton"
              virtualName="" explicitFocusOrder="0" pos="472 16 174 104" buttonText="Triangle Wave"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="note" id="648db8819c9e1ed1" memberName="note" virtualName=""
         explicitFocusOrder="0" pos="40 184 88 64" bkgCol="ff8a2be2" edTextCol="ff000000"
         edBkgCol="0" labelText="Notes" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="23.5"
         kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="sensorReading" id="4c23bd06500ca4d" memberName="sensorReading"
         virtualName="" explicitFocusOrder="0" pos="16 320 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="sensor reading&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="plusOctave" id="e0e4631d9af519da" memberName="plusOctave"
              virtualName="" explicitFocusOrder="0" pos="136 504 31 24" buttonText="+"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="octave" id="df38cd7c23ba5999" memberName="octave" virtualName=""
         explicitFocusOrder="0" pos="72 504 55 24" edTextCol="ff000000"
         edBkgCol="0" labelText="octave #" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="minusOctave" id="635122be39353565" memberName="minusOctave"
              virtualName="" explicitFocusOrder="0" pos="32 504 31 24" buttonText="-"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: sinWave_png, 3192, "../images/sinWave.png"
static const unsigned char resource_MainGUI_sinWave_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,204,0,0,1,71,1,3,0,0,0,78,14,87,31,0,0,0,6,80,76,84,69,255,255,255,0,0,255,120,192,60,
243,0,0,12,45,73,68,65,84,120,218,237,157,93,174,171,184,18,70,11,229,129,151,150,152,193,245,76,240,148,238,0,16,97,104,204,164,25,66,222,174,31,172,112,247,78,8,49,248,167,22,73,118,159,126,216,72,173,
150,78,124,188,236,170,239,43,87,114,18,16,249,189,126,175,223,235,247,250,189,62,117,213,243,128,198,205,14,13,59,193,233,190,47,51,79,108,74,15,167,187,96,244,204,182,99,230,25,77,119,134,75,188,237,
134,141,181,243,60,178,233,174,20,221,192,177,231,25,69,178,158,103,156,108,227,5,69,114,30,12,137,78,115,21,20,157,91,32,39,177,96,108,117,149,154,68,199,92,196,76,16,125,30,208,216,147,151,138,68,231,
107,27,13,148,248,215,110,208,216,218,221,86,73,118,114,114,16,237,239,211,170,57,188,8,74,204,124,159,18,121,203,177,177,223,216,102,66,65,20,232,174,250,194,198,218,97,25,171,74,226,43,232,208,214,19,
27,123,94,34,4,36,113,91,38,49,195,200,198,94,5,37,230,22,24,195,140,125,163,234,99,239,212,43,11,98,195,140,109,5,141,189,199,186,103,65,172,153,177,123,65,99,239,35,44,11,34,52,246,45,144,39,29,125,
139,139,25,136,26,169,177,111,163,42,117,153,205,40,72,19,215,231,164,170,124,28,27,123,223,175,174,9,47,76,19,207,80,95,25,186,158,216,78,80,77,89,4,214,35,35,232,154,168,46,194,228,248,220,135,69,70,
208,165,187,172,77,151,227,42,31,49,76,62,170,28,79,83,56,171,130,30,216,50,189,32,57,214,7,208,11,179,25,144,124,84,77,44,76,85,142,65,164,181,101,46,242,161,232,19,65,47,250,170,71,36,31,85,142,75,16,
245,234,248,220,134,22,161,199,54,12,11,98,117,0,173,69,232,177,52,77,142,86,152,19,2,201,106,17,122,36,68,147,99,187,252,159,160,29,91,230,67,134,154,28,123,97,38,12,144,42,122,96,114,124,32,59,128,190,
176,101,62,114,172,201,209,11,51,97,152,227,14,41,87,147,227,26,188,22,159,30,234,50,173,32,57,174,104,123,0,109,153,114,21,195,174,43,51,58,122,85,172,101,202,85,228,184,230,3,156,154,43,90,25,219,9,
50,236,26,68,112,116,173,68,165,86,120,65,78,88,189,71,208,194,208,78,144,19,86,34,56,53,173,160,101,62,83,220,163,202,67,78,205,54,10,85,185,242,40,134,93,243,7,78,205,94,80,132,158,51,89,150,63,112,
106,118,130,34,244,124,213,176,252,129,83,211,11,138,208,51,38,101,19,182,194,76,184,25,81,142,208,83,9,101,39,244,194,76,184,137,75,57,66,79,253,151,157,208,9,51,225,118,175,14,153,70,113,130,23,102,
194,109,134,61,50,141,226,4,39,204,132,91,93,119,200,52,101,39,4,89,83,79,205,96,15,61,50,77,217,9,65,254,204,1,116,203,76,83,116,66,176,46,245,212,12,68,99,153,105,138,78,8,178,161,117,205,161,85,12,
51,77,209,9,65,16,213,83,51,88,91,49,66,30,162,71,97,38,220,242,26,136,246,44,136,234,129,109,132,69,200,9,50,97,176,124,245,192,182,130,34,20,106,171,135,104,237,212,108,33,250,34,200,132,65,254,212,
3,187,23,148,156,112,11,150,229,79,61,176,59,65,201,9,95,51,44,127,242,33,116,13,209,237,1,180,23,164,139,80,7,37,255,183,194,76,184,91,90,73,23,161,241,74,254,239,133,153,112,167,133,146,46,66,92,201,
255,157,48,19,238,119,10,209,37,19,122,97,38,60,128,54,16,237,132,153,112,47,45,143,252,90,242,255,38,103,6,247,71,69,93,24,65,38,220,4,209,224,38,165,136,182,130,76,184,69,15,180,73,41,74,178,21,100,
194,205,170,154,3,232,22,162,29,203,159,210,166,108,86,102,89,169,40,56,97,147,191,250,0,218,176,82,81,66,143,28,109,32,218,11,50,225,22,61,209,254,168,40,73,39,200,9,155,244,158,14,160,27,136,238,89,
254,20,180,101,232,173,168,91,150,63,5,221,10,210,197,214,202,45,11,162,210,156,189,132,182,31,65,247,130,36,185,173,157,134,237,68,65,119,130,146,179,125,133,162,221,39,208,219,120,228,77,216,10,43,61,
209,171,249,35,105,171,130,188,19,250,87,209,213,219,232,78,88,213,139,210,145,79,206,246,16,202,31,73,157,176,170,23,239,211,161,42,85,48,161,255,60,218,64,180,19,86,112,99,97,121,84,42,10,254,119,194,
170,94,108,167,142,161,179,38,220,137,197,126,2,109,33,250,34,172,244,196,161,107,89,169,200,250,255,16,122,124,5,157,245,255,110,77,197,110,120,135,182,172,74,229,209,147,192,214,35,42,14,134,149,138,
108,233,57,132,30,24,218,11,242,255,78,58,197,150,116,143,30,144,95,243,232,145,163,141,176,8,57,65,38,220,163,167,3,232,17,149,138,44,122,247,247,139,125,161,21,20,161,189,164,123,150,191,34,186,101,
232,125,13,105,209,41,83,254,168,178,21,148,156,253,242,91,150,191,98,95,216,11,138,208,254,207,45,68,59,218,21,230,209,251,104,24,38,29,57,128,206,69,104,175,1,195,242,87,68,251,23,209,3,67,123,218,144,
230,147,179,247,123,174,244,116,194,252,159,138,136,67,126,253,4,186,130,232,125,128,115,85,207,11,51,97,42,183,30,153,38,91,122,220,27,232,238,179,104,139,187,194,44,218,10,242,127,148,63,115,0,221,51,
211,228,208,23,142,142,106,118,11,209,23,136,30,104,87,152,69,119,130,74,79,180,162,230,0,218,190,135,158,56,58,242,167,129,104,7,209,35,237,10,179,104,47,168,244,68,210,169,143,160,7,228,215,28,122,228,
232,136,212,64,180,135,232,137,54,164,57,116,156,218,142,5,241,116,4,61,34,191,126,0,109,5,37,231,117,244,133,54,164,57,116,60,67,203,164,83,29,65,79,200,175,57,180,112,116,39,40,57,175,163,221,1,244,
5,149,138,76,193,181,194,252,159,68,167,35,20,43,192,176,252,21,208,254,207,161,29,75,78,2,61,128,183,81,197,150,212,177,101,198,53,46,93,245,58,142,174,254,32,250,194,198,38,208,35,67,247,239,162,227,
204,166,171,158,19,230,255,180,139,123,84,42,50,85,239,8,122,98,99,13,249,155,63,131,182,12,157,200,159,57,128,182,172,84,36,11,238,17,116,66,44,134,161,171,127,10,221,147,138,144,12,98,246,237,71,10,
61,32,191,10,68,103,223,3,36,94,104,254,253,104,207,130,152,125,251,145,66,143,200,175,201,170,151,64,103,223,3,24,97,99,29,81,222,143,160,83,114,110,89,16,179,111,63,140,160,177,41,19,67,116,246,61,64,
139,84,154,68,219,127,6,157,170,154,134,5,49,219,136,183,12,147,90,206,187,232,142,141,165,104,203,20,122,4,157,146,94,170,244,180,204,151,183,235,47,182,204,148,225,222,64,87,35,238,143,49,186,39,111,
50,10,170,119,224,115,166,204,159,117,44,167,135,208,3,139,4,68,103,43,92,247,58,218,163,46,35,95,215,59,102,164,212,210,29,43,29,239,162,79,175,163,179,103,120,255,50,58,89,62,44,203,95,110,153,45,123,
227,146,44,154,230,243,232,234,117,116,182,75,181,47,163,147,126,77,112,178,189,185,97,126,77,36,54,137,110,62,143,150,215,209,217,127,6,74,68,8,162,147,126,77,56,41,251,238,51,177,76,135,62,124,74,163,
235,31,64,119,47,163,179,159,175,36,34,4,209,73,191,38,10,110,199,223,125,166,251,171,158,161,19,218,243,239,162,219,151,209,217,79,16,227,8,81,180,129,159,20,57,254,33,71,186,171,176,12,29,175,187,122,
27,109,32,218,225,214,60,17,161,26,162,91,214,112,229,209,241,43,233,174,34,174,122,16,125,122,27,221,64,180,199,13,105,34,57,13,68,119,172,225,42,252,219,151,99,93,69,188,32,136,174,223,70,199,105,240,
172,235,41,252,19,171,103,13,77,60,131,123,27,221,177,174,34,142,155,99,13,87,225,95,242,33,58,82,75,166,74,153,119,208,22,162,47,12,93,248,214,70,203,252,26,85,6,140,150,119,209,17,41,83,165,232,215,
33,146,164,14,162,39,134,46,124,47,201,66,180,123,17,93,248,54,150,129,189,148,99,85,42,170,122,71,208,30,162,71,214,112,21,126,22,17,137,223,177,170,215,64,116,225,75,127,13,68,119,172,84,252,65,116,
164,190,194,23,60,247,129,203,53,52,61,171,82,17,186,240,181,214,26,162,91,86,42,254,36,250,66,187,194,216,159,185,54,206,178,42,85,29,64,239,35,148,235,165,12,43,21,251,170,247,19,232,158,161,75,223,
148,223,47,43,215,198,237,21,221,177,94,175,244,251,128,8,61,50,255,119,172,215,43,162,221,107,104,207,186,158,226,111,65,28,170,205,209,159,231,252,218,227,134,52,70,15,204,255,142,181,30,69,180,127,
9,157,53,141,101,65,76,37,39,215,65,238,146,134,209,195,1,52,243,127,22,109,14,160,123,214,198,237,208,217,82,177,139,90,241,71,117,45,235,165,118,219,204,250,149,254,90,45,177,205,183,209,35,71,27,214,
198,237,74,79,22,77,127,153,152,64,119,204,255,89,191,210,223,99,38,132,224,223,69,79,28,189,211,133,99,165,39,139,222,101,162,255,4,186,99,85,138,254,236,55,94,127,69,209,3,67,251,3,232,11,43,61,89,52,
253,117,121,172,139,60,186,101,85,234,8,122,27,161,124,47,101,89,169,216,162,171,35,232,137,249,223,176,174,167,124,115,139,237,62,107,136,182,172,245,40,223,101,110,187,176,124,87,177,205,110,11,209,
83,17,237,24,122,171,233,158,117,61,202,237,91,28,107,104,182,232,142,181,30,245,17,244,192,252,79,209,99,17,237,89,87,177,157,197,179,174,71,65,119,236,104,223,198,206,177,174,167,249,8,122,163,152,66,
169,128,55,29,138,151,105,33,250,194,90,15,115,0,221,178,210,83,40,21,244,222,86,209,70,123,86,122,10,126,109,14,160,13,51,205,38,189,53,68,219,143,160,133,162,71,218,154,237,146,227,153,255,11,126,165,
247,172,139,34,228,152,9,11,126,165,119,234,219,79,83,81,244,192,76,168,160,195,8,149,142,246,158,249,117,131,86,238,214,25,70,168,116,180,183,204,52,155,41,148,123,148,134,203,44,157,175,150,153,38,12,
92,117,0,93,58,95,13,51,77,5,111,121,186,143,16,69,247,236,252,215,208,225,235,165,243,53,148,86,199,206,127,237,222,195,97,132,74,231,107,3,209,240,206,186,251,8,149,206,215,240,53,207,208,234,125,166,
61,59,95,195,100,56,246,185,144,122,119,237,142,29,114,193,60,69,211,180,175,161,45,43,61,69,229,90,220,31,109,34,212,50,255,23,209,230,0,186,101,126,13,208,39,136,86,111,93,111,153,105,2,94,209,52,1,
79,189,97,191,97,166,9,162,92,52,77,115,4,61,48,180,99,202,13,202,146,250,132,132,96,153,142,149,158,34,58,120,81,125,46,196,115,153,229,67,206,51,229,30,65,63,199,150,79,154,14,162,39,218,21,134,232,
242,73,211,51,211,208,135,107,108,151,89,62,105,90,166,92,250,72,145,45,176,124,210,88,134,166,15,82,217,46,179,92,238,13,51,13,125,124,204,118,153,229,119,226,207,12,183,236,252,215,31,43,20,160,7,230,
255,150,157,255,250,195,148,158,113,81,208,35,51,77,135,155,148,32,66,229,147,230,169,132,142,29,194,0,237,89,185,127,206,228,217,33,12,30,23,214,177,114,255,52,161,99,135,48,64,247,76,62,171,106,20,211,
24,220,41,4,203,236,153,19,20,229,174,68,240,64,60,203,228,179,110,86,65,175,62,1,143,1,92,151,233,153,19,20,229,174,41,6,15,63,92,151,233,152,19,20,249,208,103,41,134,83,105,53,183,59,136,6,207,216,124,
140,213,106,110,207,148,187,74,225,0,90,171,185,150,41,247,177,3,242,60,213,7,82,43,124,150,41,151,62,168,52,92,166,134,126,4,90,83,46,124,50,108,184,60,173,240,61,156,208,67,180,126,122,172,166,82,209,
35,147,15,124,10,112,56,151,86,115,31,115,105,104,139,79,143,117,153,150,57,65,149,207,15,160,23,221,168,232,37,122,232,73,224,150,201,103,97,170,202,93,228,136,158,127,110,88,14,23,180,42,159,37,210,
12,125,91,102,229,153,28,85,244,50,0,61,235,254,190,76,189,250,116,76,62,11,218,115,180,94,125,122,38,159,101,34,132,190,47,83,175,62,247,236,217,1,105,2,61,141,123,89,166,94,125,238,80,85,62,119,40,122,
252,185,156,156,254,102,239,25,106,93,62,158,163,239,218,214,171,207,125,132,103,37,10,149,240,101,54,189,250,220,102,211,61,40,103,92,71,151,101,90,29,125,89,179,163,107,194,48,180,93,215,170,107,162,
190,176,18,165,239,228,185,204,43,211,68,51,177,58,113,70,100,49,95,211,85,58,250,182,58,16,200,155,38,174,12,221,124,5,241,4,170,207,153,5,242,59,39,149,103,232,218,221,255,35,137,57,235,232,239,93,156,
28,67,127,143,53,23,150,152,25,212,137,235,61,144,232,250,154,239,60,161,196,156,174,108,58,51,65,244,121,168,102,96,134,218,75,67,114,248,181,141,243,8,209,118,60,205,36,49,179,88,18,72,227,208,78,238,
99,103,139,20,57,255,111,38,129,108,230,191,161,183,190,34,57,207,72,22,231,25,237,230,107,58,79,209,167,121,70,185,177,51,218,77,5,119,178,140,29,88,116,152,93,237,76,85,246,21,201,43,92,34,155,178,153,
49,89,42,104,195,255,192,249,254,43,191,215,239,245,123,253,94,63,114,253,31,110,51,28,63,189,4,121,159,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* MainGUI::sinWave_png = (const char*) resource_MainGUI_sinWave_png;
const int MainGUI::sinWave_pngSize = 3192;

// JUCER_RESOURCE: squareWave_png, 2214, "../images/squareWave.png"
static const unsigned char resource_MainGUI_squareWave_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,208,0,0,0,99,8,2,0,0,0,164,22,198,56,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,4,
103,65,77,65,0,0,177,143,11,252,97,5,0,0,0,9,112,72,89,115,0,0,14,195,0,0,14,195,1,199,111,168,100,0,0,8,59,73,68,65,84,120,94,237,212,49,142,92,73,18,5,193,185,255,165,185,64,195,92,216,17,40,12,192,
98,68,84,154,70,7,133,120,217,168,255,207,175,231,121,158,231,35,222,7,247,121,158,231,67,222,7,247,121,158,231,67,222,7,247,249,47,254,249,26,6,175,101,198,23,48,120,182,41,87,122,179,47,96,240,114,198,
124,1,131,215,50,227,11,24,60,219,251,224,126,154,193,203,25,243,5,12,94,203,140,47,96,240,108,239,131,251,105,6,47,103,204,23,48,120,45,51,190,128,193,179,189,15,238,167,25,188,156,49,95,192,224,181,
204,248,2,6,207,246,62,184,159,102,240,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,
48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,
164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,
162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,
4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,
138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,
108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,
41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,
168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,
206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,
162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,
186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,
83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,
114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,
153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,
229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,
46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,
150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,
174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,
114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,
153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,
229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,
46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,
150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,
174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,
114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,
153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,
229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,
46,103,76,212,19,76,138,186,150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,
150,25,81,79,48,41,234,108,83,174,244,102,81,79,48,41,234,114,198,68,61,193,164,168,107,153,17,245,4,147,162,206,54,229,74,111,22,245,4,147,162,46,103,76,212,19,76,138,186,150,25,81,79,48,105,60,231,254,
152,242,7,112,90,212,19,76,138,186,156,49,81,79,48,233,40,35,79,48,105,60,231,254,152,242,7,112,90,212,19,76,138,186,156,49,81,79,48,233,40,35,79,48,105,60,231,254,248,247,31,192,127,249,219,92,115,130,
73,81,151,51,38,234,9,38,29,101,228,9,38,141,231,220,31,255,255,143,49,28,116,130,73,167,153,122,130,73,71,25,121,130,73,227,57,247,199,251,224,254,113,38,157,102,234,9,38,93,100,225,243,247,188,15,238,
31,103,210,105,166,62,207,243,91,255,254,169,248,1,253,109,174,57,193,164,211,76,125,158,231,183,222,79,229,143,243,77,58,205,212,231,121,126,235,253,84,158,231,121,62,228,125,112,159,231,121,62,228,125,
112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,
121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,228,125,112,159,231,121,62,226,215,175,255,1,69,226,47,86,110,134,2,236,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* MainGUI::squareWave_png = (const char*) resource_MainGUI_squareWave_png;
const int MainGUI::squareWave_pngSize = 2214;


//[EndFile] You can add extra defines here...
//[/EndFile]
