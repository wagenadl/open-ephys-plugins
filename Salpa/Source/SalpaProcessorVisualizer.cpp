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

  content.asymdur->setValue(proc->t_asym);
  content.blankdur->setValue(proc->t_blankdur);
  content.potdur->setValue(proc->t_potblank);
  content.negrail->setValue(proc->v_neg_rail);
  content.posrail->setValue(proc->v_pos_rail);
  content.digithr->setValue(proc->absthr);
  content.absenable->setToggleState(proc->useabsthr ? true : false,
                                    juce::dontSendNotification);
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