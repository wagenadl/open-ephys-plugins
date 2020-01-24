/*
  ------------------------------------------------------------------

  This file is part of the Open Ephys GUI
  Copyright (C) 2016 Open Ephys

  ------------------------------------------------------------------

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SalpaProcessorEditor.h"
#include "SalpaProcessor.h"
#include "SalpaProcessorVisualizer.h"

static const int EDITOR_DESIRED_WIDTH = 150;


SalpaProcessorEditor::SalpaProcessorEditor(SalpaProcessor* parentNode,
                                           bool useDefaultParameterEditors):
  VisualizerEditor(parentNode, useDefaultParameterEditors) {
  tabText = "SALPA";

  // Open Ephys Plugin Generator will insert generated code for editor here. Don't edit this section.
  //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_BEGIN]

  //m_contentLookAndFeel = new LOOKANDFEELCLASSNAME();
  //content.setLookAndFeel (m_contentLookAndFeel);
  addAndMakeVisible (&content);
  content.toBack(); // to be able to see parameters components
  setDesiredWidth (EDITOR_DESIRED_WIDTH);

  //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_END]

  content.processor = parentNode;
  printf("Filling salpa editor boxes\n");
  for (int k=0; k<SalpaProcessor::PARAMETER_COUNT; k++)
    reflectParameter(k);
  printf("Done filling salpa editor boxes\n");
  // should fill visualizer as well


  ////Most used buttons are UtilityButton, which shows a simple button with text and ElectrodeButton, which is an on-off button which displays a channel.
  //exampleButton = new UtilityButton ("Button text", Font ("Default", 15, Font::plain));
  //exampleButton->setBounds (10, 30, 50, 50); //Set position and size (X, Y, XSize, YSize)
  //exampleButton->addListener (this); //Specify which class will implement the listener methods, in the case of editors, always make the editor the listener
  //exampleButton->setClickingTogglesState (true); //True for a on-off toggleable button, false for a single-click monostable button
  //exampleButton->setTooltip ("Mouseover tooltip text");
  //addAndMakeVisible (exampleButton);
}


SalpaProcessorEditor::~SalpaProcessorEditor() {
}


void SalpaProcessorEditor::resized() {
  // Don't edit this section.
  //[OPENEPHYS_EDITOR_PRE_RESIZE_SECTION_BEGIN]

  VisualizerEditor::resized();

  const int xPosInitial = 2;
  const int yPosIntiial = 23;
  const int contentWidth = (isMerger() || isSplitter() || isUtility())
    ? getWidth() - xPosInitial * 2
    : channelSelector->isVisible()
    ? channelSelector->getX() - xPosInitial * 2 - 5
    : drawerButton->getX() - xPosInitial * 2;
  content.setBounds (xPosInitial, yPosIntiial,
                     contentWidth, getHeight() - yPosIntiial - 7);

  //[OPENEPHYS_EDITOR_PRE_RESIZE_SECTION_END]
}


/**
   The listener method that reacts to the button click. The same method is called for all buttons
   on the editor, so the button variable, which cointains a pointer to the button that called the method
   has to be checked to know which function to perform.
*/
void SalpaProcessorEditor::buttonEvent(Button *button) {
  //if (button == exampleButton)
  //{
  //    //Do Stuff

  //    //a typical  example:
  //    if (button->getToggleState()) //Button is pressed
  //    {
  //        getProcessor()->setParameter(0, 1);
  //    }
  //    else
  //    {
  //        getProcessor()->setParameter(0, 0);
  //    }
  //}
}


/**
   The listener method that reacts to the changes of the particular slider of the editor.
   We should check which slider was changed and perofrm appropriate functions.
*/
void SalpaProcessorEditor::sliderEvent(Slider *slider) {
}


Visualizer *SalpaProcessorEditor::createNewCanvas() {
  SalpaProcessor *processor = dynamic_cast<SalpaProcessor *>(getProcessor());
  printf("Salpa - new canvas - %p\n", processor);
  Visualizer *vis = new SalpaProcessorVisualizer(processor);
  printf("vis = %p\n", vis);
  return vis;
}

void SalpaProcessorEditor::saveCustomParameters(XmlElement *xml) {
  VisualizerEditor::saveCustomParameters(xml);
  SalpaProcessor *processor = dynamic_cast<SalpaProcessor *>(getProcessor());
  if (!processor) {
    printf("SALPAPROCESSOREDITOR - No PROCESSOR\n");
    return;
  }
  processor->saveMyParameters(xml);
}

void SalpaProcessorEditor::loadCustomParameters(XmlElement *xml) {
  VisualizerEditor::loadCustomParameters(xml);
  SalpaProcessor *processor = dynamic_cast<SalpaProcessor *>(getProcessor());
  if (!processor) {
    printf("SALPAPROCESSOREDITOR - NO PROCESSOR\n");
    return;
  }
  processor->loadMyParameters(xml);
  for (int k=0; k<SalpaProcessor::PARAMETER_COUNT; k++)
    reflectParameter(k);
}

void SalpaProcessorEditor::reflectParameter(int idx) {
  SalpaProcessor *prc = content.processor;
  if (!prc) {
    printf("SALPAPROCESSOREDITOR - NO PROCESSOR\n");
    return;
  }
  switch (idx) {
  case SalpaProcessor::PARAM_T_AHEAD:
    content.lookahead->setValue(prc->t_ahead);
    break;
  case SalpaProcessor::PARAM_TAU:
    content.tau->setValue(prc->tau);
    break;
  case SalpaProcessor::PARAM_RELTHR:
    content.relthr->setValue(prc->relthr);
    break;
  case SalpaProcessor::PARAM_T_BLANKDUR:
    content.blankdur->setValue(prc->t_blankdur);
    break;
  default: { // send other parameters to visualizer
    Visualizer *vis0 = canvas;
    SalpaProcessorVisualizer *vis
      = dynamic_cast<SalpaProcessorVisualizer *>(vis0);
    if (vis)
      vis->reflectParameter(idx);
  } break;
  }
}
