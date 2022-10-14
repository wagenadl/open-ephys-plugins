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


SalpaPlugin::SalpaPlugin(): GenericProcessor("SALPA") {
  auto addparam = [this](char const *name, int min, int max, int dflt) {
    addIntParameter(Parameter::STREAM_SCOPE, name, "x", dflt, min, max, false);
  };
  addparam("V_NEG_RAIL", -32767, -1, -3000);
  addparam("V_POS_RAIL", 1, 32767, 3000);  
  addparam("T_POTBLANK", 1, 100, 15);  
  addparam("T_BLANKDUR", 1, 100, 5);  
  addparam("N_TOOPOOR", 0, 100, 5);  
  addparam("T_AHEAD", 0, 100, 5);  
  addparam("TAU", 1, 1000, 30);  
  addparam("RELTHR", 1, 100, 3);  
  addparam("ABSTHR", 0, 32700, 0);  
  addBooleanParameter(Parameter::STREAM_SCOPE, "USEABSTHR", "x", false, false);
  addparam("T_ASYM", 0, 1000, 5);  
  addparam("EVENTCHANNEL", -1, 7, 0);  
  addparam("V_ZERO", -32700, 32700, 0);  
}


SalpaPlugin::~SalpaPlugin() {
}


AudioProcessorEditor* SalpaPlugin::createEditor() {
  editor = std::make_unique<SalpaPluginEditor>(this);
  return editor.get();
}

bool SalpaPlugin::startAcquisition() {
  std::cerr << "SalpaPlugin::startAcquisition\n";
  for (auto stream: getDataStreams()) {
    if ((*stream)["enable_stream"]) {
      SalpaModule *module = modules[stream->getStreamId()];
      module->startAcquisition();
    }
  }
  return true;
}

bool SalpaPlugin::stopAcquisition() {
  return true;
}

void SalpaPlugin::dropFitters() {
  for (auto stream: getDataStreams()) {
    if ((*stream)["enable_stream"]) {
      SalpaModule *module = modules[stream->getStreamId()];
      module->dropFitters();
    }
  }
}

void SalpaPlugin::updateSettings() {
  modules.update(getDataStreams());
  
  for (auto stream: getDataStreams()) {
    const uint16 streamId = stream->getStreamId();
    SalpaModule *module = modules[streamId];
    module->updateSettings(streamId, continuousChannels);
    parameterValueChanged(stream->getParameter("V_NEG_RAIL"));
    parameterValueChanged(stream->getParameter("V_POS_RAIL"));
    parameterValueChanged(stream->getParameter("T_POTBLANK"));
    parameterValueChanged(stream->getParameter("T_BLANKDUR"));
    parameterValueChanged(stream->getParameter("N_TOOPOOR"));
    parameterValueChanged(stream->getParameter("T_AHEAD"));
    parameterValueChanged(stream->getParameter("TAU"));
    parameterValueChanged(stream->getParameter("RELTHR"));
    parameterValueChanged(stream->getParameter("ABSTHR"));
    parameterValueChanged(stream->getParameter("T_ASYM"));
    parameterValueChanged(stream->getParameter("EVENTCHANNEL"));
    parameterValueChanged(stream->getParameter("V_ZERO"));

    EventChannel::Settings s{
      EventChannel::Type::TTL,
      "SALPA trigger output",
      "Triggers when SALPA blanks/deblanks",
      "SALPA.blank",
      getDataStream(streamId)
    };

    eventChannels.add(new EventChannel(s));
    eventChannels.getLast()->addProcessor(processorInfo.get());
    modules[streamId]->outputEventChannel = eventChannels.getLast();
  }

}

void SalpaPlugin::parameterValueChanged(Parameter *param) {
  auto name = param->getName();
  int stream = param->getStreamId();
  auto value = param->getValue();
  if (name.equalsIgnoreCase("V_NEG_RAIL"))
    modules[stream]->v_neg_rail = value;
  else if (name.equalsIgnoreCase("V_POS_RAIL"))
    modules[stream]->v_pos_rail = value;
  else if (name.equalsIgnoreCase("T_POTBLANK"))
    modules[stream]->t_potblank = value;
  else if (name.equalsIgnoreCase("T_BLANKDUR"))
    modules[stream]->t_blankdur = value;
  else if (name.equalsIgnoreCase("N_TOOPOOR"))
    modules[stream]->n_toopoor = value;
  else if (name.equalsIgnoreCase("T_AHEAD"))
    modules[stream]->t_ahead = value;
  else if (name.equalsIgnoreCase("TAU"))
    modules[stream]->tau = value;
  else if (name.equalsIgnoreCase("RELTHR"))
    modules[stream]->relthr = value;
  else if (name.equalsIgnoreCase("ABSTHR"))
    modules[stream]->absthr = value;
  else if (name.equalsIgnoreCase("USEABSTHR"))
    modules[stream]->useabsthr = value;
  else if (name.equalsIgnoreCase("T_ASYM"))
    modules[stream]->t_asym = value;
  else if (name.equalsIgnoreCase("EVENTCHANNEL"))
    modules[stream]->eventchannel = value;
  else if (name.equalsIgnoreCase("V_ZERO"))
    modules[stream]->v_zero = value;
  modules[stream]->dropFitters();

}
  
void SalpaPlugin::process(AudioBuffer<float> &buffer) {
  checkForEvents(true);
  for (auto stream: getDataStreams()) {
    if ((*stream)["enable_stream"]) {
      const uint16 streamId = stream->getStreamId();
      const int64 firstSampleInBlock = getFirstSampleNumberForBlock(streamId);
      const uint32 numSamplesInBlock = getNumSamplesInBlock(streamId);
      SalpaModule *module = modules[streamId];
      std::list<SalpaModule::OutputEvent> events 
        = module->process(buffer, firstSampleInBlock, numSamplesInBlock);
      for (auto &evt: events) {
         TTLEventPtr event = TTLEvent::createTTLEvent(evt.channelptr,
                                                      evt.time,
                                                      evt.line,
                                                      evt.state);
         addEvent(event, evt.reltime);
      }
    }
  }
}


void SalpaPlugin::handleTTLEvent(TTLEventPtr event) {
  const bool eventState = event->getState() ? true : false;
  const int eventChannel = event->getLine();
  const uint16 eventStreamId = event->getChannelInfo()->getStreamId();
  // const int eventSourceNodeId = event->getChannelInfo()->getSourceNodeId();
  const int64 eventTime = event->getSampleNumber();
  // const int firstInBlock = getFirstSampleNumberForBlock(eventStreamId);
  modules[eventStreamId]->handleEvent(eventChannel, eventState, eventTime);
}


void SalpaPlugin::handleSpike(SpikePtr event) {
}


void SalpaPlugin::handleBroadcastMessage(String message) {
}


void SalpaPlugin::saveCustomParametersToXml(XmlElement* parentElement) {
}


void SalpaPlugin::loadCustomParametersFromXml(XmlElement* parentElement) {
}
