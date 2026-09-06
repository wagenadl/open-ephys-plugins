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
  constexpr int x0 = 10;
  constexpr int dx = 180;
  constexpr int y0 = 25;
  constexpr int dy = 20;
  desiredWidth = 3*dx;
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "V_neg_rail", x0, y0);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "V_pos_rail", x0, y0+dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "V_zero",     x0, y0+2*dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "RelThr",     x0, y0+3*dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "AbsThr",     x0, y0+4*dy);

  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "Tau",        x0+dx, y0);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "T_pot_blank", x0+dx, y0+dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "T_blank_dur", x0+dx, y0+2*dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "T_ahead",    x0+dx, y0+3*dy);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "T_asym",     x0+dx, y0+4*dy);

  addToggleParameterEditor(Parameter::STREAM_SCOPE, "UseAbsThr",        x0+2*dx, y0);
  addBoundedValueParameterEditor(Parameter::STREAM_SCOPE, "N_too_poor",  x0+2*dx, y0+dy);
  addComboBoxParameterEditor(Parameter::STREAM_SCOPE, "EventChannel",   x0+2*dx, y0+2*dy);

}

SalpaPluginEditor::~SalpaPluginEditor() {
}
