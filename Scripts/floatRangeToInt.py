#!/usr/bin/python3

import sys
import json
import copy

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def handleParam(parameter):
   if "sourceRanges" in parameter["source"]:
      wholeRangeStart = 0
      wholeRangeEnd = 128
      if parameter["source"]["midi"]["id"][".type"] == "MidiNRPN":
         wholeRangeEnd = 128 * 128
      if "sourceValueRange" in parameter["source"]["midi"]:
         wholeRangeStart = parameter["source"]["midi"]["sourceValueRange"]["from"]
         wholeRangeEnd = parameter["source"]["midi"]["sourceValueRange"]["to"] + 1
      wholeRange = wholeRangeEnd - wholeRangeStart
      for idx, sourceRange in enumerate(parameter["source"]["sourceRanges"]):
         if "range" in sourceRange:
            sourceRange["range"]["from"] = int(wholeRangeStart + (sourceRange["range"]["from"] * wholeRange))
            sourceRange["range"]["to"] = int(wholeRangeStart + (sourceRange["range"]["to"] * wholeRange) - 1)
         else:
            numElements = len(parameter["source"]["sourceRanges"])
            sourceRange["range"] = {"from" : 0, "to" : 0}
            sourceRange["range"]["from"] = int(wholeRangeStart + (idx * wholeRange / numElements))
            sourceRange["range"]["to"] = int(wholeRangeStart + ((idx + 1) * wholeRange / numElements) - 1)
      parameter["source"]["midi"]["sourceRanges"] = copy.deepcopy(parameter["source"]["sourceRanges"])
      del parameter["source"]["sourceRanges"]
      

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
