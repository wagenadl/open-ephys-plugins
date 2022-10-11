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

#include "SalpaPlugin.h"

#include "SalpaPluginEditor.h"


SalpaPlugin::SalpaPlugin()
    : GenericProcessor("SALPA")
{

}


SalpaPlugin::~SalpaPlugin()
{

}


AudioProcessorEditor* SalpaPlugin::createEditor()
{
    editor = std::make_unique<SalpaPluginEditor>(this);
    return editor.get();
}


void SalpaPlugin::updateSettings()
{


}


void SalpaPlugin::process(AudioBuffer<float>& buffer)
{

    checkForEvents(true);

}


void SalpaPlugin::handleTTLEvent(TTLEventPtr event)
{

}


void SalpaPlugin::handleSpike(SpikePtr event)
{

}


void SalpaPlugin::handleBroadcastMessage(String message)
{

}


void SalpaPlugin::saveCustomParametersToXml(XmlElement* parentElement)
{

}


void SalpaPlugin::loadCustomParametersFromXml(XmlElement* parentElement)
{

}
