#!/usr/bin/python3

import sys
import json
import copy 

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   sound_section["sourceRanges"]["sampleSlots"].clear()
   for i in range(0, 128):
      element = {
         "name": str(i),
         "range" : {"from": i * 128, "to": i * 128}
      }
      sound_section["sourceRanges"]["sampleSlots"].append(element)

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
