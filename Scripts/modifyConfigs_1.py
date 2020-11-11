#!/usr/bin/python3

import sys
import json

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   sound_section["global"] = dict()
   sound_section["global"]["components"] = dict()
   sound_section["global"]["parameters"] = dict()
   sound_section["engines"] = [{ "name" : "MainEngine" }]
   sound_section["engines"][0]["components"] = dict()
   sound_section["engines"][0]["parameters"] = dict()

   for voice in sound_section["voices"]:
      voice["engineId"] = 0

   #parameter keys should be unique
   testDict = dict()
   for idx, parameter in enumerate(sound_section["parameters"]):
      if parameter["name"] in testDict:
         #print("Multiple keys: " + parameter["name"])
         orig_idx = testDict[parameter["name"]]
         parameter["name"] += "_2"
         sound_section["parameters"][orig_idx]["name"] += "_1"
      testDict[parameter["name"]] = idx
   del testDict

   #category keys should be unique
   testDict = dict()
   for idx, category in enumerate(sound_section["parameter_categories"]):
      if category["category_name"] in testDict:
         #print("Multiple Category name: " + category["category_name"])
         orig_idx = testDict[category["category_name"]]
         category["category_name"] += "_2"
         sound_section["parameter_categories"][orig_idx]["category_name"] += "_1"
      testDict[category["category_name"]] = idx
   del testDict


   for category in sound_section["parameter_categories"]:
      sound_section["engines"][0]["components"][category["category_name"]] = { "parameterIds" : [] }
      for paramIndex in category["parameter_ids"]:
         sound_section["parameters"][paramIndex]["component"] = category["category_name"]
         sound_section["engines"][0]["components"][category["category_name"]]["parameterIds"].append(sound_section["parameters"][paramIndex]["name"])
   del sound_section["parameter_categories"]

   for parameter in sound_section["parameters"]:
      if "nrpn" in parameter["midi"]:
         if  len(parameter["midi"]["nrpn"]) == 2:
            parameter["source"] = {".type" : "MidiNRPN", "idMsb" : parameter["midi"]["nrpn"][0], "idLsb" : parameter["midi"]["nrpn"][1]}
         else:
            print("ERROR: NRPN has not got 2 parameters ids: '" +  parameter["name"] + "'")
            sys.exit(1)
      else:
         if "cc" in parameter["midi"]:
            if  len(parameter["midi"]["cc"]) == 1:
               parameter["source"] = {".type" : "MidiControlChange", "id" : parameter["midi"]["cc"][0]}
            if  len(parameter["midi"]["cc"]) == 2:
               parameter["source"] = {".type" : "MidiControlChangeHighRes", "idMsb" : parameter["midi"]["cc"][0], "idLsb" : parameter["midi"]["cc"][1] }

      del parameter["midi"]

      if "ranges" in parameter:
         parameter["source"]["ranges"] = parameter["ranges"]
         del parameter["ranges"]

      if not "component" in parameter:
         parameter["component"] = "Default"
         if not "Default" in sound_section["engines"][0]["components"]:
            sound_section["engines"][0]["components"]["Default"] = { "parameterIds" : [] }
         sound_section["engines"][0]["components"]["Default"]["parameterIds"].append(parameter["name"])

      sound_section["engines"][0]["parameters"][parameter["name"]] = parameter
      del sound_section["engines"][0]["parameters"][parameter["name"]]["name"]
      

   del sound_section["parameters"]

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      morph_sound_section(data["soundSection"])
   print(json.dumps(data, indent=2))
