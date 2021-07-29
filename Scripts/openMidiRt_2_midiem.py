#!/usr/bin/python3

import sys
import json
import copy 

jsonBase = """
{
  "version": "0.0.0",
  "manufacturer": "Generic",
  "productName": "1 Voice",
  "reactsToTransportCommands": true,
  "soundSection": {
    "defaultInstrumentType": "InstrumentPerVoice",
    "voices": [
      {
        "name": "1",
        "engineId": 0,
        "midiChannel": 1
      }
    ],
    "engines": [
      {
        "name": "MainEngine",
        "components": [],
        "parameters": []
      }
    ]
  }
}      
"""


if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

with open(sys.argv[1]) as json_file:
   source_data = json.load(json_file)
   data = json.loads(jsonBase)

   data["manufacturer"] = source_data["device"]["manufacturer"]
   data["productName"] = source_data["device"]["displayName"]

   if "controlChangeCommands" in source_data:
      for ccCommand in source_data["controlChangeCommands"]:
         param = {   "name": ccCommand["name"], \
                     "description" : "", \
                     "type": "continous", \
                     "source": { \
                        "midi": { \
                           "id": {".type": "MidiControlChange", "id" : ccCommand["controlChangeNumber"]} \
                        } \
                     } \
                  }
         if ccCommand["valueRange"]["min"] != 0 or ccCommand["valueRange"]["max"] != 127:
            param["source"]["midi"]["sourceValueRange"] = { "from": ccCommand["valueRange"]["min"], "to": ccCommand["valueRange"]["max"]}

         data["soundSection"]["engines"][0]["parameters"].append(param)

   if "nrpnCommands" in source_data:
      for nrpnCommand in source_data["nrpnCommands"]:
         param = {   "name": nrpnCommand["name"], \
                     "description" : "", \
                     "type": "continous", \
                     "source": { \
                        "midi": { \
                           "id": {".type": "MidiNRPN", "idMsb" : nrpnCommand["parameterNumber"] >> 7, "idLsb" : nrpnCommand["parameterNumber"] & ((1 << 8) - 1) } \
                        } \
                     } \
                  }
         if nrpnCommand["valueRange"]["min"] != 0 or nrpnCommand["valueRange"]["max"] != 127:
            param["source"]["midi"]["sourceValueRange"] = { "from": nrpnCommand["valueRange"]["min"] * 128, "to": nrpnCommand["valueRange"]["max"] *128}

         data["soundSection"]["engines"][0]["parameters"].append(param)

   print(json.dumps(data, indent=2))
