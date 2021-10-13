#!/usr/bin/python3

import sys
import json
import copy

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def handleParam(parameter):
   if "sourceValueRange" in parameter["source"]["midi"] and parameter["type"] == "list":
      del parameter["source"]["midi"]["sourceValueRange"]

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      if "global" in data["soundSection"]:
         for parameter in data["soundSection"]["global"]["parameters"]:
            handleParam(parameter)

      if "engines" in data["soundSection"]:
         for engine in data["soundSection"]["engines"]:
            for parameter in engine["parameters"]:
               handleParam(parameter)

   print(json.dumps(data, indent=2))
