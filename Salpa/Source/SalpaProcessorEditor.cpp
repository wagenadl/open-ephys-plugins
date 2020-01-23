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
  content.relthr->setValue(parentNode->relthr);
  content.tau->setValue(parentNode->tau);
  content.lookahead->setValue(parentNode->t_ahead);
  if (parentNode->eventchannel>=0)
    content.eventChannel->setText(String(parentNode->eventchannel + 1));
  else
    content.eventChannel->setText("-");
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
  XmlElement *mainNode = xml->createNewChildElement("VALUES");

  auto saveParameter = [this, processor, mainNode](int idx, float value) {
    auto parameter = processor->getParameterObject(idx);
    mainNode->setAttribute(parameter->getName(), value);
  };

  saveParameter(SalpaProcessor::PARAM_V_NEG_RAIL, processor->v_neg_rail);
  saveParameter(SalpaProcessor::PARAM_V_POS_RAIL, processor->v_pos_rail);
  saveParameter(SalpaProcessor::PARAM_T_POTBLANK, processor->t_potblank);
  saveParameter(SalpaProcessor::PARAM_T_BLANKDUR, processor->t_blankdur);
  saveParameter(SalpaProcessor::PARAM_N_TOOPOOR, processor->n_toopoor);
  saveParameter(SalpaProcessor::PARAM_T_AHEAD, processor->t_ahead);
  saveParameter(SalpaProcessor::PARAM_TAU, processor->tau);
  saveParameter(SalpaProcessor::PARAM_RELTHR, processor->relthr);
  saveParameter(SalpaProcessor::PARAM_ABSTHR, processor->absthr);
  saveParameter(SalpaProcessor::PARAM_USEABSTHR, processor->useabsthr);
  saveParameter(SalpaProcessor::PARAM_T_ASYM, processor->t_asym);
  saveParameter(SalpaProcessor::PARAM_EVENTCHANNEL, processor->eventchannel);
  saveParameter(SalpaProcessor::PARAM_V_ZERO, processor->v_zero);
}

SalpaProcessorVisualizerContentComponent *SalpaProcessorEditor::visualizerContent() {
  Visualizer *vis0 = canvas;
  SalpaProcessorVisualizer *vis = dynamic_cast<SalpaProcessorVisualizer *>(vis0);
  if (vis)
    return &vis->content;
  else
    return 0;
}

void SalpaProcessorEditor::loadCustomParameters(XmlElement *xml) {
  VisualizerEditor::loadCustomParameters(xml);
  SalpaProcessor *processor = dynamic_cast<SalpaProcessor *>(getProcessor());
  if (!processor) {
    printf("SALPAPROCESSOREDITOR - NO PROCESSOR\n");
    return;
  }

  SalpaProcessorVisualizerContentComponent *viscont = visualizerContent();
  
  bool ok = false;
  forEachXmlChildElementWithTagName(*xml, mainNode, "VALUES") {
    auto loadParameter = [this, processor, mainNode](int idx) {
      auto parameter = processor->getParameterObject(idx);
      return mainNode->getDoubleAttribute(parameter->getName());
    };
    ok = true;

    float v_neg_rail = loadParameter(SalpaProcessor::PARAM_V_NEG_RAIL);
    if (viscont)
      viscont->negrail->setValue(v_neg_rail);
    else
      processor->setParameter(SalpaProcessor::PARAM_V_NEG_RAIL, v_neg_rail);

    float v_pos_rail = loadParameter(SalpaProcessor::PARAM_V_POS_RAIL);
    if (viscont)
      viscont->posrail->setValue(v_pos_rail);
    else
      processor->setParameter(SalpaProcessor::PARAM_V_POS_RAIL, v_pos_rail);

    float t_potblank = loadParameter(SalpaProcessor::PARAM_T_POTBLANK);
    if (viscont)
      viscont->potdur->setValue(t_potblank);
    else
      processor->setParameter(SalpaProcessor::PARAM_T_POTBLANK, t_potblank);
    
    float t_blankdur = loadParameter(SalpaProcessor::PARAM_T_BLANKDUR);
    if (viscont)
      viscont->blankdur->setValue(t_blankdur);
    else
      processor->setParameter(SalpaProcessor::PARAM_T_BLANKDUR, t_blankdur);

    // n_toopoor is not exposed to the gui
    
    float t_ahead = loadParameter(SalpaProcessor::PARAM_T_AHEAD);
    content.lookahead->setValue(t_ahead);

    float tau = loadParameter(SalpaProcessor::PARAM_TAU);
    content.tau->setValue(tau);

    float relthr = loadParameter(SalpaProcessor::PARAM_RELTHR);
    content.relthr->setValue(relthr);

    float absthr = loadParameter(SalpaProcessor::PARAM_ABSTHR);
    if (viscont)
      viscont->digithr->setValue(absthr);
    else
      processor->setParameter(SalpaProcessor::PARAM_ABSTHR, absthr);

    bool useabs = loadParameter(SalpaProcessor::PARAM_USEABSTHR);
    if (viscont)
      viscont->absenable->setToggleState(useabs, juce::sendNotification);
    else
      processor->setParameter(SalpaProcessor::PARAM_USEABSTHR, useabs);

    float t_asym = loadParameter(SalpaProcessor::PARAM_T_ASYM);
    if (viscont)
      viscont->asymdur->setValue(t_asym);
    else
      processor->setParameter(SalpaProcessor::PARAM_T_ASYM, t_asym);
    
    int evtch = loadParameter(SalpaProcessor::PARAM_EVENTCHANNEL);
    if (evtch>=0)
      content.eventChannel->setText(String(evtch + 1));
    else
      content.eventChannel->setText(String("-"));

    // v_zero is not exposed to the gui
    
  }
  if (!ok) {
    printf("SALPAPROCESSOREDITOR - NO XML CONTENT\n");
  }
}
