#!/usr/bin/python3

import sys
import json
import copy 

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
