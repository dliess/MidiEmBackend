#!/usr/bin/python3

import sys
import json
import csv
import copy 

jsonBase = """
{
  "version": "0.0.0",
  "manufacturer": "Generic",
  "productName": "1 Voice",
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

with open(sys.argv[1]) as csv_file:
   data = json.loads(jsonBase)
   csv_data = csv.DictReader(csv_file)
   componentNames = []
   for csv_row in csv_data:
      data["manufacturer"] = csv_row['\ufeffmanufacturer']
      data["productName"] = csv_row["device"]
      if not csv_row["section"] in componentNames:
         data["soundSection"]["engines"][0]["components"].append({"name" : csv_row["section"]})
         componentNames.append(csv_row["section"])
      param = {   "name": csv_row["parameter_name"], \
                  "description" : csv_row["parameter_description"], \
                  "type": "continous", \
                  "component": csv_row["section"], \
                  "source": { \
                     "midi": { \
                        "id": { } \
                     } \
                  } \
               }

      midiParamId = None
      if csv_row["nrpn_msb"] != "" or csv_row["nrpn_lsb"] != "":
         midiParamId = {".type": "MidiNRPN", "idMsb" : csv_row["nrpn_msb"], "idLsb" : csv_row["nrpn_lsb"]}

      if csv_row["cc_msb"] != "":
         if csv_row["cc_lsb"] == "":
            midiParamId = {".type": "MidiControlChange", "id" : csv_row["cc_msb"]}
         else:
            midiParamId = {".type": "MidiControlChangeHighRes", "idMsb" : csv_row["cc_msb"], "idLsb" : csv_row["cc_lsb"]}


         param["source"]["midi"]["id"] = midiParamId

      data["soundSection"]["engines"][0]["parameters"].append(param)
      print("THE ROW: " + str(csv_row))
   print(json.dumps(data, indent=2))
