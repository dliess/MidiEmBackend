#!/usr/bin/python3

import sys
import json
import copy 

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   for engine in sound_section["engines"]:
      for parameter in engine["parameters"]:
         if "sourceRanges" in parameter["source"]["midi"]:
            parameter["source"]["sourceRanges"] = []
            for idx, sourceRange in enumerate(parameter["source"]["midi"]["sourceRanges"]):
               newSourceRange = dict()
               newSourceRange["name"] = sourceRange["name"]
               newSourceRange["range"] = dict()
               resolution = 128.0
               if "sourceValueRange" in parameter["source"]["midi"]:
                  resolution = parameter["source"]["midi"]["sourceValueRange"]["to"] - parameter["source"]["midi"]["sourceValueRange"]["from"] + 1

               newSourceRange["range"]["from"] = sourceRange["range"][0] / resolution
               if idx != len(parameter["source"]["midi"]["sourceRanges"]) - 1:
                  newSourceRange["range"]["to"] = parameter["source"]["midi"]["sourceRanges"][idx + 1]["range"][0] / resolution
               else:
                  newSourceRange["range"]["to"] = 1.0
               parameter["source"]["sourceRanges"].append(newSourceRange)
            del parameter["source"]["midi"]["sourceRanges"]

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
