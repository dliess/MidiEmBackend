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

with open(sys.argv[1]) as csv_file:
   data = json.loads(jsonBase)
   csv_data = csv.DictReader(csv_file)
   componentNames = []
   for csv_row in csv_data:
      if '\ufeffmanufacturer' in csv_row:
         data["manufacturer"] = csv_row['\ufeffmanufacturer']
      if 'manufacturer' in csv_row:
         data["manufacturer"] = csv_row['manufacturer']
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

      valueRange = None
      midiParamId = None
      if csv_row["cc_msb"] != "":
         if csv_row["cc_min_value"] == "":
            csv_row["cc_min_value"] = "0"
         if csv_row["cc_max_value"] == "":
            csv_row["cc_max_value"] = "127"
         valueRange = int(csv_row["cc_max_value"]) - int(csv_row["cc_min_value"]) + 1
         if csv_row["cc_lsb"] == "":
            midiParamId = {".type": "MidiControlChange", "id" : int(csv_row["cc_msb"])}
            if csv_row["cc_min_value"] != "0" or csv_row["cc_max_value"] != "127":
               param["source"]["midi"]["sourceValueRange"] = { "from": int(csv_row["cc_min_value"]), "to": int(csv_row["cc_max_value"])}
         else:
            midiParamId = {".type": "MidiControlChangeHighRes", "idMsb" : int(csv_row["cc_msb"]), "idLsb" : int(csv_row["cc_lsb"])}
      elif csv_row["nrpn_msb"] != "" or csv_row["nrpn_lsb"] != "":
         if csv_row["nrpn_min_value"] == "":
            csv_row["nrpn_min_value"] = "0"
         if csv_row["nrpn_max_value"] == "":
            csv_row["nrpn_max_value"] = "127"
         valueRange = int(csv_row["nrpn_max_value"]) - int(csv_row["nrpn_min_value"]) + 1
         midiParamId = {".type": "MidiNRPN", "idMsb" : int(csv_row["nrpn_msb"]), "idLsb" : int(csv_row["nrpn_lsb"])}
         if csv_row["nrpn_min_value"] != "0" or csv_row["nrpn_max_value"] != "127":
            param["source"]["midi"]["sourceValueRange"] = { "from": int(csv_row["nrpn_min_value"]), "to": int(csv_row["nrpn_max_value"])}

      if "usage" in csv_row:
         if csv_row["usage"] != "":
            param["source"]["sourceRanges"] = []
            for usage in csv_row["usage"].split(";"):
               range, name = usage.strip().split(":")
               min = None
               max = None
               if "-" in range:
                  min,max = range.split("-")
               else:
                  min = range
                  max = range

               minVal = None
               maxVal = None
               if "0b" in min:
                  minVal = float((int(min.strip(), 2)) / valueRange)
                  maxVal = float((int(max.strip(), 2) + 1) / valueRange)
               else:
                  minVal = float((int(min.strip())) / valueRange)
                  maxVal = float((int(max.strip()) + 1) / valueRange)
               param["source"]["sourceRanges"].append({"name": name.strip(), "range": {"from": minVal, "to": maxVal}})

      param["source"]["midi"]["id"] = midiParamId

      if csv_row["orientation"] == "0-based":
         param["type"] = "continous"
      elif csv_row["orientation"] == "centered":
         param["type"] = "continous-bipolar"

      data["soundSection"]["engines"][0]["parameters"].append(param)
      #print("THE ROW: " + str(csv_row))
   print(json.dumps(data, indent=2))
