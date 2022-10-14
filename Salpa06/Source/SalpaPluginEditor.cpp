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
  desiredWidth = 550;
  constexpr int x0 = 10;
  constexpr int dx = 100;
  constexpr int y0 = 20;
  constexpr int dy = 34;
  addTextBoxParameterEditor("V_NEG_RAIL", x0, y0);
  addTextBoxParameterEditor("V_POS_RAIL", x0, y0+dy);
  addTextBoxParameterEditor("V_ZERO",     x0, y0+2*dy);

  addTextBoxParameterEditor("T_POTBLANK", x0+dx, y0);
  addTextBoxParameterEditor("T_BLANKDUR", x0+dx, y0+dy);
  addTextBoxParameterEditor("T_AHEAD",    x0+dx, y0+2*dy);

  addTextBoxParameterEditor("RELTHR",     x0+2*dx, y0);
  addTextBoxParameterEditor("ABSTHR",     x0+2*dx, y0+dy);
  addTextBoxParameterEditor("USEABSTHR", x0+2*dx, y0+2*dy);

  addTextBoxParameterEditor("TAU",           x0+3*dx, y0);
  addTextBoxParameterEditor("N_TOOPOOR",     x0+3*dx, y0+dy);
  addComboBoxParameterEditor("EVENTCHANNEL", x0+3*dx, y0+2*dy);

  addTextBoxParameterEditor("T_ASYM",           x0+4*dx, y0);
}

SalpaPluginEditor::~SalpaPluginEditor() {
}
