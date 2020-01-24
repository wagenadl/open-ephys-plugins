/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SalpaProcessor.h"
//[/Headers]

#include "SalpaProcessorContentComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SalpaProcessorContentComponent::SalpaProcessorContentComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
  processor = 0;
    //[/Constructor_pre]

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Time cst")));
    label->setTooltip (TRANS("Time constant (samples)"));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Max dev.")));
    label2->setTooltip (TRANS("Threshold for depegging (units of RMS noise)\n"));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Look ahead")));
    label3->setTooltip (TRANS("Look ahead (samples)\n"));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (tau = new Slider ("tau"));
    tau->setTooltip (TRANS("Time constant (samples)"));
    tau->setRange (1, 1000, 1);
    tau->setSliderStyle (Slider::IncDecButtons);
    tau->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    tau->addListener (this);

    addAndMakeVisible (relthr = new Slider ("relthr"));
    relthr->setTooltip (TRANS("Threshold for depegging (units of RMS noise)\n"));
    relthr->setRange (1, 20, 0.1);
    relthr->setSliderStyle (Slider::IncDecButtons);
    relthr->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    relthr->addListener (this);

    addAndMakeVisible (lookahead = new Slider ("lookahead"));
    lookahead->setTooltip (TRANS("Look ahead (samples)\n"));
    lookahead->setRange (1, 200, 1);
    lookahead->setSliderStyle (Slider::IncDecButtons);
    lookahead->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    lookahead->addListener (this);

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Post blank")));
    label4->setTooltip (TRANS("Post-artifact additional blanking \n"
    "(samples)\n"));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (blankdur = new Slider ("blankdur"));
    blankdur->setTooltip (TRANS("Post-artifact additional blanking \n"
    "(samples)\n"));
    blankdur->setRange (1, 200, 1);
    blankdur->setSliderStyle (Slider::IncDecButtons);
    blankdur->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    blankdur->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (150, 100);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SalpaProcessorContentComponent::~SalpaProcessorContentComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    tau = nullptr;
    relthr = nullptr;
    lookahead = nullptr;
    label4 = nullptr;
    blankdur = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SalpaProcessorContentComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SalpaProcessorContentComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label->setBounds (0, proportionOfHeight (0.0000f), proportionOfWidth (0.5000f), 24);
    label2->setBounds (0, proportionOfHeight (0.2500f), proportionOfWidth (0.5000f), 24);
    label3->setBounds (0, proportionOfHeight (0.5000f), proportionOfWidth (0.5000f), 24);
    tau->setBounds (proportionOfWidth (1.0000f) - proportionOfWidth (0.5000f), proportionOfHeight (0.0200f), proportionOfWidth (0.5000f), 20);
    relthr->setBounds (proportionOfWidth (1.0000f) - proportionOfWidth (0.5000f), proportionOfHeight (0.2700f), proportionOfWidth (0.5000f), 20);
    lookahead->setBounds (proportionOfWidth (1.0000f) - proportionOfWidth (0.5000f), proportionOfHeight (0.5200f), proportionOfWidth (0.5000f), 20);
    label4->setBounds (0, proportionOfHeight (0.7500f), proportionOfWidth (0.5000f), 24);
    blankdur->setBounds (proportionOfWidth (1.0000f) - proportionOfWidth (0.5000f), proportionOfHeight (0.7500f), proportionOfWidth (0.5000f), 20);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SalpaProcessorContentComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
  if (!processor)
    return;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == tau)
    {
        //[UserSliderCode_tau] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_TAU,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_tau]
    }
    else if (sliderThatWasMoved == relthr)
    {
        //[UserSliderCode_relthr] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_RELTHR,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_relthr]
    }
    else if (sliderThatWasMoved == lookahead)
    {
        //[UserSliderCode_lookahead] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_T_AHEAD,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_lookahead]
    }
    else if (sliderThatWasMoved == blankdur)
    {
        //[UserSliderCode_blankdur] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_T_BLANKDUR,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_blankdur]
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

<JUCER_COMPONENT documentType="Component" className="SalpaProcessorContentComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="150" initialHeight="100">
  <BACKGROUND backgroundColour="ffffff"/>
  <LABEL name="new label" id="b42076347613b878" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 0% 50% 24" tooltip="Time constant (samples)"
         edTextCol="ff000000" edBkgCol="0" labelText="Time cst" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="9fa18e94f8d3b639" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="0 25% 50% 24" tooltip="Threshold for depegging (units of RMS noise)&#10;"
         edTextCol="ff000000" edBkgCol="0" labelText="Max dev." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b15157f8041b5ec2" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="0 50% 50% 24" tooltip="Look ahead (samples)&#10;"
         edTextCol="ff000000" edBkgCol="0" labelText="Look ahead" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="tau" id="f85c73add7da17e4" memberName="tau" virtualName=""
          explicitFocusOrder="0" pos="100%r 2% 50% 20" tooltip="Time constant (samples)"
          min="1" max="1000" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="relthr" id="439f63233ceb3c43" memberName="relthr" virtualName=""
          explicitFocusOrder="0" pos="100%r 27% 50% 20" tooltip="Threshold for depegging (units of RMS noise)&#10;"
          min="1" max="20" int="0.10000000000000000555" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="35"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="lookahead" id="f5f599cd96784081" memberName="lookahead"
          virtualName="" explicitFocusOrder="0" pos="100%r 52% 50% 20"
          tooltip="Look ahead (samples)&#10;" min="1" max="200" int="1"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="35" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="accf2126b817f9ba" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="0 75% 50% 24" tooltip="Post-artifact additional blanking &#10;(samples)&#10;"
         edTextCol="ff000000" edBkgCol="0" labelText="Post blank" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="blankdur" id="75a212c460f3d2d4" memberName="blankdur" virtualName=""
          explicitFocusOrder="0" pos="100%r 75% 50% 20" tooltip="Post-artifact additional blanking &#10;(samples)&#10;"
          min="1" max="200" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
