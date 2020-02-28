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
#include "FutureThresholdProcessor.h"
//[/Headers]

#include "FutureThresholdProcessorContentComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FutureThresholdProcessorContentComponent::FutureThresholdProcessorContentComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Source")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (eventChannel = new ComboBox ("EventChannel"));
    eventChannel->setTooltip (TRANS("Output event channel"));
    eventChannel->setExplicitFocusOrder (2);
    eventChannel->setEditableText (false);
    eventChannel->setJustificationType (Justification::centredLeft);
    eventChannel->setTextWhenNothingSelected (String());
    eventChannel->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    eventChannel->addItem (TRANS("1"), 1);
    eventChannel->addItem (TRANS("2"), 2);
    eventChannel->addItem (TRANS("3"), 3);
    eventChannel->addItem (TRANS("4"), 4);
    eventChannel->addItem (TRANS("5"), 5);
    eventChannel->addItem (TRANS("6"), 6);
    eventChannel->addItem (TRANS("7"), 7);
    eventChannel->addItem (TRANS("8"), 8);
    eventChannel->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Event")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (triggerThreshold = new Slider ("TriggerThreshold"));
    triggerThreshold->setTooltip (TRANS("Threshold for event detection"));
    triggerThreshold->setExplicitFocusOrder (3);
    triggerThreshold->setRange (-32700, 32700, 1);
    triggerThreshold->setSliderStyle (Slider::LinearHorizontal);
    triggerThreshold->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    triggerThreshold->addListener (this);

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("+")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (resetThreshold = new Slider ("ResetThreshold"));
    resetThreshold->setTooltip (TRANS("Threshold for resetting the trigger"));
    resetThreshold->setExplicitFocusOrder (4);
    resetThreshold->setRange (-32700, 32700, 1);
    resetThreshold->setSliderStyle (Slider::LinearHorizontal);
    resetThreshold->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    resetThreshold->addListener (this);

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("-")));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Future")));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (futureMS = new Slider ("FutureMS"));
    futureMS->setTooltip (TRANS("Propogation delay (samples)"));
    futureMS->setExplicitFocusOrder (4);
    futureMS->setRange (0, 1000, 1);
    futureMS->setSliderStyle (Slider::LinearHorizontal);
    futureMS->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    futureMS->addListener (this);

    addAndMakeVisible (adcChannel = new Slider ("adcChannel"));
    adcChannel->setRange (1, 136, 1);
    adcChannel->setSliderStyle (Slider::IncDecButtons);
    adcChannel->setTextBoxStyle (Slider::TextBoxLeft, false, 30, 20);
    adcChannel->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (200, 100);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FutureThresholdProcessorContentComponent::~FutureThresholdProcessorContentComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    eventChannel = nullptr;
    label2 = nullptr;
    triggerThreshold = nullptr;
    label3 = nullptr;
    resetThreshold = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    futureMS = nullptr;
    adcChannel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FutureThresholdProcessorContentComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FutureThresholdProcessorContentComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label->setBounds (0, proportionOfHeight (0.0000f), proportionOfWidth (0.2500f), proportionOfHeight (0.2400f));
    eventChannel->setBounds (getWidth() - proportionOfWidth (0.2000f), proportionOfHeight (0.0000f), proportionOfWidth (0.2000f), proportionOfHeight (0.2000f));
    label2->setBounds (proportionOfWidth (0.6000f), proportionOfHeight (0.0000f), proportionOfWidth (0.2000f), proportionOfHeight (0.2400f));
    triggerThreshold->setBounds (20, proportionOfHeight (0.2500f), getWidth() - 20, proportionOfHeight (0.2000f));
    label3->setBounds (0, proportionOfHeight (0.2500f), 20, proportionOfHeight (0.2400f));
    resetThreshold->setBounds (20, proportionOfHeight (0.5000f), getWidth() - 20, proportionOfHeight (0.2000f));
    label4->setBounds (0, proportionOfHeight (0.5000f), 20, 24);
    label5->setBounds (0, proportionOfHeight (0.7500f), 50, proportionOfHeight (0.2400f));
    futureMS->setBounds (getWidth() - (getWidth() - 50), proportionOfHeight (0.7500f), getWidth() - 50, proportionOfHeight (0.2000f));
    adcChannel->setBounds (proportionOfWidth (0.6000f) - proportionOfWidth (0.3500f), 0, proportionOfWidth (0.3500f), 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FutureThresholdProcessorContentComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
  String s = comboBoxThatHasChanged->getText();
  int ch = s.getIntValue() - 1;
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == eventChannel)
    {
        //[UserComboBoxCode_eventChannel] -- add your combo box handling code here..
      if (processor)
        processor->setParameter(FutureThresholdProcessor::PARAM_OUTPUT_CHANNEL, ch);
        //[/UserComboBoxCode_eventChannel]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FutureThresholdProcessorContentComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == triggerThreshold)
    {
        //[UserSliderCode_triggerThreshold] -- add your slider handling code here..
      if (processor)
        processor->setParameter(FutureThresholdProcessor::PARAM_TRIGGER_THRESHOLD,
                                sliderThatWasMoved->getValue());
        //[/UserSliderCode_triggerThreshold]
    }
    else if (sliderThatWasMoved == resetThreshold)
    {
        //[UserSliderCode_resetThreshold] -- add your slider handling code here..
      if (processor)
        processor->setParameter(FutureThresholdProcessor::PARAM_RESET_THRESHOLD,
                                sliderThatWasMoved->getValue());
        //[/UserSliderCode_resetThreshold]
    }
    else if (sliderThatWasMoved == futureMS)
    {
        //[UserSliderCode_futureMS] -- add your slider handling code here..
      if (processor)
        processor->setParameter(FutureThresholdProcessor::PARAM_DELAY_SAMPS,
                                sliderThatWasMoved->getValue());
        //[/UserSliderCode_futureMS]
    }
    else if (sliderThatWasMoved == adcChannel)
    {
        //[UserSliderCode_adcChannel] -- add your slider handling code here..
      if (processor)
        processor->setParameter(FutureThresholdProcessor::PARAM_INPUT_CHANNEL,
                                sliderThatWasMoved->getValue() - 1);
        //[/UserSliderCode_adcChannel]
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

<JUCER_COMPONENT documentType="Component" className="FutureThresholdProcessorContentComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="200" initialHeight="100">
  <BACKGROUND backgroundColour="ffffff"/>
  <LABEL name="new label" id="b7d039b79515964c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 0% 25% 24%" edTextCol="ff000000"
         edBkgCol="0" labelText="Source" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="EventChannel" id="bf7663568d447a07" memberName="eventChannel"
            virtualName="" explicitFocusOrder="2" pos="0Rr 0% 20% 20%" tooltip="Output event channel"
            editable="0" layout="33" items="1&#10;2&#10;3&#10;4&#10;5&#10;6&#10;7&#10;8"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="1812967a52dbd2d0" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="60% 0% 20% 24%" edTextCol="ff000000"
         edBkgCol="0" labelText="Event" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="TriggerThreshold" id="48402addfc350acb" memberName="triggerThreshold"
          virtualName="" explicitFocusOrder="3" pos="20 25% 20M 20%" tooltip="Threshold for event detection"
          min="-32700" max="32700" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="new label" id="ce272e92d526a20f" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="0 25% 20 24%" edTextCol="ff000000"
         edBkgCol="0" labelText="+" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="ResetThreshold" id="386c9c0917432d48" memberName="resetThreshold"
          virtualName="" explicitFocusOrder="4" pos="20 50% 20M 20%" tooltip="Threshold for resetting the trigger"
          min="-32700" max="32700" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="new label" id="cfb0672dde7980dc" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="0 50% 20 24" edTextCol="ff000000"
         edBkgCol="0" labelText="-" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f2dfd544c38fa2ab" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="0 75% 50 24%" edTextCol="ff000000"
         edBkgCol="0" labelText="Future" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="FutureMS" id="6f57554c0e7f5d3b" memberName="futureMS" virtualName=""
          explicitFocusOrder="4" pos="0Rr 75% 50M 20%" tooltip="Propogation delay (samples)"
          min="0" max="1000" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="adcChannel" id="3f5f9767511773ac" memberName="adcChannel"
          virtualName="" explicitFocusOrder="0" pos="60%r 0 35% 24" min="1"
          max="136" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
