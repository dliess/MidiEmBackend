#!/usr/bin/python3

import sys
import json
import copy 

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   paramArray = []
   for i in range(308):
      paramArray.append({ ".type": "Reserved", "offset": i + 32, "size": 1 })
   for engine in sound_section["engines"]:
      for parameter in engine["parameters"]:
         if "description" in parameter:
            digits = [int(s) for s in parameter["description"].split() if s.isdigit()]
            paramArray[digits[0] - 32] = {".type": "Parameter", "offset": digits[0], "component": parameter["component"], "parameter": parameter["name"] }
            #print(parameter["component"] + "::" + parameter["name"] + "DIGIT: " + str(digits[0] - 32))
   for element in paramArray:
      sound_section["parameterDumpAnswer"]["sysexMsg"].append(element)

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
