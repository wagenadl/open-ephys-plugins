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

#ifndef __JUCE_HEADER_EB9BE0CF46404FC__
#define __JUCE_HEADER_EB9BE0CF46404FC__

//[Headers]     -- You can add your own extra header files here --
#include <EditorHeaders.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FutureThresholdProcessorContentComponent  : public Component,
                                                  public ComboBoxListener,
                                                  public SliderListener
{
public:
    //==============================================================================
    FutureThresholdProcessorContentComponent ();
    ~FutureThresholdProcessorContentComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    // Make an editor to be friendly class of this content component,
    // so the editor will have access to all methods and variables of this component.
    friend class FutureThresholdProcessorEditor;
  class FutureThresholdProcessor *processor;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;
    ScopedPointer<ComboBox> eventChannel;
    ScopedPointer<Label> label2;
    ScopedPointer<Slider> triggerThreshold;
    ScopedPointer<Label> label3;
    ScopedPointer<Slider> resetThreshold;
    ScopedPointer<Label> label4;
    ScopedPointer<Label> label5;
    ScopedPointer<Slider> futureMS;
    ScopedPointer<Slider> adcChannel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FutureThresholdProcessorContentComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_EB9BE0CF46404FC__
