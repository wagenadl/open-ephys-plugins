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

#ifndef __JUCE_HEADER_7A97AB6A3608CAD2__
#define __JUCE_HEADER_7A97AB6A3608CAD2__

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
class SalpaProcessorVisualizerContentComponent  : public Component,
                                                  public SliderListener,
                                                  public ButtonListener
{
public:
    //==============================================================================
    SalpaProcessorVisualizerContentComponent ();
    ~SalpaProcessorVisualizerContentComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    // Make an editor to be friendly class of this content component,
    // so the editor will have access to all methods and variables of this component.
    friend class SalpaProcessorVisualizer;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> label4;
    ScopedPointer<Label> label5;
    ScopedPointer<Label> label6;
    ScopedPointer<Slider> asymdur;
    ScopedPointer<Slider> blankdur;
    ScopedPointer<Slider> potdur;
    ScopedPointer<Slider> negrail;
    ScopedPointer<Slider> posrail;
    ScopedPointer<Slider> digithr;
    ScopedPointer<ToggleButton> absenable;
    ScopedPointer<TextButton> resetTraining;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SalpaProcessorVisualizerContentComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_7A97AB6A3608CAD2__
