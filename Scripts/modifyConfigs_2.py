#!/usr/bin/python3

import sys
import json
import copy 

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   for paramkey in sound_section["engines"][0]["parameters"]:
      parameter = sound_section["engines"][0]["parameters"][paramkey]
      savedRanges = None
      if "ranges" in parameter["source"]:
         savedRanges = copy.deepcopy(parameter["source"]["ranges"])
         del parameter["source"]["ranges"]

      savedId = copy.deepcopy(parameter["source"])
      del parameter["source"]

      parameter["source"] = dict()
      parameter["source"]["midi"] = dict()
      parameter["source"]["midi"]["id"] = savedId
      if savedRanges:
         parameter["source"]["midi"]["sourceRanges"] = savedRanges

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
