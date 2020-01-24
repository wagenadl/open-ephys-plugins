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

#include "SalpaProcessorVisualizer.h"
#include "SalpaProcessor.h"


SalpaProcessorVisualizer::SalpaProcessorVisualizer(SalpaProcessor *proc) {
  content.processor = proc;
  // Open Ephys Plugin Generator will insert generated code for editor here. Don't edit this section.
  //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_BEGIN]

  //m_contentLookAndFeel = new LOOKANDFEELCLASSNAME();
  //content.setLookAndFeel (m_contentLookAndFeel);
  addAndMakeVisible (&content);

  //[OPENEPHYS_EDITOR_PRE_CONSTRUCTOR_SECTION_END]
  for (int k=0; k<SalpaProcessor::PARAMETER_COUNT; k++)
    reflectParameter(k);
}

SalpaProcessorVisualizer::~SalpaProcessorVisualizer() {
}

void SalpaProcessorVisualizer::resized() {
  content.setBounds(getLocalBounds());
}

void SalpaProcessorVisualizer::refreshState() {
}

void SalpaProcessorVisualizer::update() {
}

void SalpaProcessorVisualizer::refresh() {
}

void SalpaProcessorVisualizer::beginAnimation() {
  startCallbacks();
}

void SalpaProcessorVisualizer::endAnimation() {
  stopCallbacks();
}

void SalpaProcessorVisualizer::setParameter(int parameter, float newValue) {
}

void SalpaProcessorVisualizer::setParameter (int parameter,
                                             int val1, int val2,
                                             float newValue) {
}

void SalpaProcessorVisualizer::reflectParameter(int idx) {
  SalpaProcessor *prc = content.processor;
  if (!prc) {
    printf("SALPAPROCESSORVISUALIZER - NO PROCESSOR\n");
    return;
  }
  switch (idx) {
  case SalpaProcessor::PARAM_V_NEG_RAIL:
    content.negrail->setValue(prc->v_neg_rail);
    break;
  case SalpaProcessor::PARAM_V_POS_RAIL:
    content.posrail->setValue(prc->v_pos_rail);
    break;
  case SalpaProcessor::PARAM_T_POTBLANK:
    content.potdur->setValue(prc->t_potblank);
    break;
  case SalpaProcessor::PARAM_N_TOOPOOR:
    // n_toopoor is not exposed to the gui
    break;
  case SalpaProcessor::PARAM_ABSTHR:
    content.digithr->setValue(prc->absthr);
    break;
  case SalpaProcessor::PARAM_USEABSTHR:
      content.absenable->setToggleState(prc->useabsthr,
                                        juce::sendNotification);
    break;
  case SalpaProcessor::PARAM_T_ASYM:
    content.asymdur->setValue(prc->t_asym);
    break;
  case SalpaProcessor::PARAM_EVENTCHANNEL: {
    int evtch = prc->eventchannel;
    if (evtch>=0)
      content.eventChannel->setText(String(evtch + 1));
    else
      content.eventChannel->setText(String("-"));
  } break;
  case SalpaProcessor::PARAM_V_ZERO:
    // v_zero is not exposed to the gui
    break;
  default:
    printf("SALPAPROCESSORVISUALIZER - UNKNOWN PARAMETER INDEX %i\n", idx);
    break;
  }
}
