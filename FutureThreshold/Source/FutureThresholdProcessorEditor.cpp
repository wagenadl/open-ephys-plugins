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

#include "FutureThresholdProcessorEditor.h"
#include "FutureThresholdProcessor.h"

static const int EDITOR_DESIRED_WIDTH = 200;


FutureThresholdProcessorEditor::FutureThresholdProcessorEditor(FutureThresholdProcessor *parentNode,
                                                               bool useDefaultParameterEditors):
  GenericEditor (parentNode, useDefaultParameterEditors)
{
    // Open Ephys Plugin Generator will insert generated code for editor here. Don't edit this section.
    //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_BEGIN]

    //m_contentLookAndFeel = new LOOKANDFEELCLASSNAME();
    //content.setLookAndFeel (m_contentLookAndFeel);
    addAndMakeVisible (&content);
    content.toBack(); // to be able to see parameters components
    setDesiredWidth (EDITOR_DESIRED_WIDTH);

    //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_END]

    content.processor = parentNode;
    if (parentNode->input_channel < 0)
      content.adcChannel->setValue(-1);
    else
      content.adcChannel->setValue(parentNode->input_channel + 1);
    content.eventChannel->setText(String(parentNode->output_channel + 1));
    content.triggerThreshold->setValue(parentNode->trigger_threshold);
    content.resetThreshold->setValue(parentNode->reset_threshold);
    content.futureMS->setValue(parentNode->future_samps);
}


FutureThresholdProcessorEditor::~FutureThresholdProcessorEditor() {
}

void FutureThresholdProcessorEditor::saveCustomParameters(XmlElement *xml) {
  GenericEditor::saveCustomParameters(xml);
  FutureThresholdProcessor *processor
    = dynamic_cast<FutureThresholdProcessor *>(getProcessor());
  if (!processor) {
    printf("FUTURETHRESHOLDPROCESSOREDITOR - No PROCESSOR\n");
    return;
  }
  XmlElement *mainNode = xml->createNewChildElement("VALUES");

  auto saveParameter = [this, processor, mainNode](int idx, float value) {
    auto parameter = processor->getParameterObject(idx);
    mainNode->setAttribute(parameter->getName(), value);
  };

  saveParameter(FutureThresholdProcessor::PARAM_INPUT_CHANNEL,
                processor->input_channel);
  saveParameter(FutureThresholdProcessor::PARAM_OUTPUT_CHANNEL,
                processor->output_channel);
  saveParameter(FutureThresholdProcessor::PARAM_TRIGGER_THRESHOLD,
                processor->trigger_threshold);
  saveParameter(FutureThresholdProcessor::PARAM_RESET_THRESHOLD,
                processor->reset_threshold);
  saveParameter(FutureThresholdProcessor::PARAM_DELAY_SAMPS,
                processor->future_samps);
}

void FutureThresholdProcessorEditor::loadCustomParameters(XmlElement *xml) {
  GenericEditor::loadCustomParameters(xml);
  FutureThresholdProcessor *processor
    = dynamic_cast<FutureThresholdProcessor *>(getProcessor());
  if (!processor) {
    printf("FUTURETHRESHOLDPROCESSOREDITOR - NO PROCESSOR\n");
    return;
  }
  
  bool ok = false;
  forEachXmlChildElementWithTagName(*xml, mainNode, "VALUES") {
    auto loadParameter = [this, processor, mainNode](int idx) {
      auto parameter = processor->getParameterObject(idx);
      return mainNode->getDoubleAttribute(parameter->getName());
    };
    ok = true;
    
    int inpch = loadParameter(FutureThresholdProcessor::PARAM_INPUT_CHANNEL);
    if (inpch>=0)
      content.adcChannel->setValue(inpch+1);
    else
      content.adcChannel->setValue(0);
    int outpch = loadParameter(FutureThresholdProcessor::PARAM_OUTPUT_CHANNEL);
    if (outpch>=0)
      content.eventChannel->setText(String(outpch+1));
    else
      content.eventChannel->setText("-");

    float tt = loadParameter(FutureThresholdProcessor::PARAM_TRIGGER_THRESHOLD);
    content.triggerThreshold->setValue(tt);

    float rt = loadParameter(FutureThresholdProcessor::PARAM_RESET_THRESHOLD);
    content.resetThreshold->setValue(rt);
    
    float fut = loadParameter(FutureThresholdProcessor::PARAM_DELAY_SAMPS);
    content.futureMS->setValue(fut);
  }
  if (!ok) {
    printf("FUTURETHRESHOLDPROCESSOREDITOR - NO XML CONTENT\n");
  }
}



void FutureThresholdProcessorEditor::resized() {
    // Don't edit this section.
    //[OPENEPHYS_EDITOR_PRE_RESIZE_SECTION_BEGIN]

    GenericEditor::resized();

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
