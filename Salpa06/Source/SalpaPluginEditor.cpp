/*
------------------------------------------------------------------

This file is part of the SALPA plugin to Open Ephys GUI
Copyright (C) 2022 Daniel Wagenaar

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

#include "SalpaPluginEditor.h"

SalpaPluginEditor::SalpaPluginEditor(GenericProcessor* parentNode):
  GenericEditor(parentNode) {
  desiredWidth = 450;
  addTextBoxParameterEditor("V_NEG_RAIL", 10, 22);
  addTextBoxParameterEditor("V_POS_RAIL", 10, 62);
  // plenty more
}

SalpaPluginEditor::~SalpaPluginEditor() {
}
