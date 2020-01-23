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

#include "SalpaProcessorVisualizerContentComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SalpaProcessorVisualizerContentComponent::SalpaProcessorVisualizerContentComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("Absolute max. deviation")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (absenable = new ToggleButton ("new toggle button"));
    absenable->setButtonText (String());
    absenable->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Asymmetry window width")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Min. blank duration")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Forced peg min. duration")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Negative pegging threshold")));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Positive pegging threshold")));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (asymdur = new Slider ("asymdur"));
    asymdur->setTooltip (TRANS("\n"));
    asymdur->setRange (1, 1000, 1);
    asymdur->setSliderStyle (Slider::IncDecButtons);
    asymdur->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    asymdur->addListener (this);

    addAndMakeVisible (blankdur = new Slider ("blankdur"));
    blankdur->setTooltip (TRANS("\n"));
    blankdur->setRange (1, 1000, 1);
    blankdur->setSliderStyle (Slider::IncDecButtons);
    blankdur->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    blankdur->addListener (this);

    addAndMakeVisible (potdur = new Slider ("potdur"));
    potdur->setTooltip (TRANS("\n"));
    potdur->setRange (1, 1000, 1);
    potdur->setSliderStyle (Slider::IncDecButtons);
    potdur->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    potdur->addListener (this);

    addAndMakeVisible (negrail = new Slider ("negrail"));
    negrail->setTooltip (TRANS("\n"));
    negrail->setRange (-32700, 0, 100);
    negrail->setSliderStyle (Slider::IncDecButtons);
    negrail->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    negrail->addListener (this);

    addAndMakeVisible (posrail = new Slider ("posrail"));
    posrail->setTooltip (TRANS("\n"));
    posrail->setRange (0, 32700, 100);
    posrail->setSliderStyle (Slider::IncDecButtons);
    posrail->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    posrail->addListener (this);

    addAndMakeVisible (digithr = new Slider ("digithr"));
    digithr->setTooltip (TRANS("\n"));
    digithr->setRange (0, 32700, 100);
    digithr->setSliderStyle (Slider::IncDecButtons);
    digithr->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    digithr->addListener (this);

    addAndMakeVisible (resetTraining = new TextButton ("resetTraining"));
    resetTraining->setButtonText (TRANS("Reset noise data"));
    resetTraining->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SalpaProcessorVisualizerContentComponent::~SalpaProcessorVisualizerContentComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label6 = nullptr;
    absenable = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    asymdur = nullptr;
    blankdur = nullptr;
    potdur = nullptr;
    negrail = nullptr;
    posrail = nullptr;
    digithr = nullptr;
    resetTraining = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SalpaProcessorVisualizerContentComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
  printf("paint - %p\n", processor);
  GenericEditor *ed = processor->editor;
  printf("paint --- %p\n", ed);
    ColourGradient editorBg = ed->getBackgroundGradient();
    g.fillAll(editorBg.getColourAtPosition(0.5)); // roughly matches editor back    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SalpaProcessorVisualizerContentComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label6->setBounds (0, 125, 200, 22);
    absenable->setBounds (158, 127, 25, 20);
    label->setBounds (0, 0, 200, 20);
    label2->setBounds (0, 25, 200, 20);
    label3->setBounds (0, 50, 200, 20);
    label4->setBounds (0, 75, 200, 21);
    label5->setBounds (0, 100, 200, 21);
    asymdur->setBounds (180, 2, 120, 20);
    blankdur->setBounds (180, 27, 120, 20);
    potdur->setBounds (180, 52, 120, 20);
    negrail->setBounds (180, 77, 120, 20);
    posrail->setBounds (180, 102, 120, 20);
    digithr->setBounds (180, 127, 120, 20);
    resetTraining->setBounds (144, 272, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SalpaProcessorVisualizerContentComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == absenable)
    {
        //[UserButtonCode_absenable] -- add your button handler code here..
      processor->setParameter(SalpaProcessor::PARAM_USEABSTHR,
                              buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_absenable]
    }
    else if (buttonThatWasClicked == resetTraining)
    {
        //[UserButtonCode_resetTraining] -- add your button handler code here..
      processor->forgetTraining();
        //[/UserButtonCode_resetTraining]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SalpaProcessorVisualizerContentComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == asymdur)
    {
        //[UserSliderCode_asymdur] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_T_ASYM,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_asymdur]
    }
    else if (sliderThatWasMoved == blankdur)
    {
        //[UserSliderCode_blankdur] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_T_BLANKDUR,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_blankdur]
    }
    else if (sliderThatWasMoved == potdur)
    {
        //[UserSliderCode_potdur] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_T_POTBLANK,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_potdur]
    }
    else if (sliderThatWasMoved == negrail)
    {
        //[UserSliderCode_negrail] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_V_NEG_RAIL,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_negrail]
    }
    else if (sliderThatWasMoved == posrail)
    {
        //[UserSliderCode_posrail] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_V_POS_RAIL,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_posrail]
    }
    else if (sliderThatWasMoved == digithr)
    {
        //[UserSliderCode_digithr] -- add your slider handling code here..
      processor->setParameter(SalpaProcessor::PARAM_ABSTHR,
                              sliderThatWasMoved->getValue());
        //[/UserSliderCode_digithr]
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

<JUCER_COMPONENT documentType="Component" className="SalpaProcessorVisualizerContentComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="300" initialHeight="300">
  <BACKGROUND backgroundColour="ffffff"/>
  <LABEL name="new label" id="3727047bd1062461" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="0 125 200 22" edTextCol="ff000000"
         edBkgCol="0" labelText="Absolute max. deviation" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="233d5ff191355d43" memberName="absenable"
                virtualName="" explicitFocusOrder="0" pos="158 127 25 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="13086f4fe30b5dfa" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 0 200 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Asymmetry window width" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b1a676d8a0b634a1" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="0 25 200 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Min. blank duration" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4375229ef2bdd986" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="0 50 200 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Forced peg min. duration" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="bde4fcfbf9b4e4c1" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="0 75 200 21" edTextCol="ff000000"
         edBkgCol="0" labelText="Negative pegging threshold" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="9d369357711b5590" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="0 100 200 21" edTextCol="ff000000"
         edBkgCol="0" labelText="Positive pegging threshold" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="asymdur" id="f85c73add7da17e4" memberName="asymdur" virtualName=""
          explicitFocusOrder="0" pos="180 2 120 20" tooltip="&#10;" min="1"
          max="1000" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="blankdur" id="4fb4512ccfd726e0" memberName="blankdur" virtualName=""
          explicitFocusOrder="0" pos="180 27 120 20" tooltip="&#10;" min="1"
          max="1000" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="potdur" id="bc8bbbd0dc4c6dff" memberName="potdur" virtualName=""
          explicitFocusOrder="0" pos="180 52 120 20" tooltip="&#10;" min="1"
          max="1000" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="negrail" id="a6e47e4098af7cc9" memberName="negrail" virtualName=""
          explicitFocusOrder="0" pos="180 77 120 20" tooltip="&#10;" min="-32700"
          max="0" int="100" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="posrail" id="dd0fceae505ec2f7" memberName="posrail" virtualName=""
          explicitFocusOrder="0" pos="180 102 120 20" tooltip="&#10;" min="0"
          max="32700" int="100" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="digithr" id="35e088a063c6f9c8" memberName="digithr" virtualName=""
          explicitFocusOrder="0" pos="180 127 120 20" tooltip="&#10;" min="0"
          max="32700" int="100" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TEXTBUTTON name="resetTraining" id="a7adfa5e540555ed" memberName="resetTraining"
              virtualName="" explicitFocusOrder="0" pos="144 272 150 24" buttonText="Reset noise data"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
