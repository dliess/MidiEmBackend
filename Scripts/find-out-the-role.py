#!/usr/bin/python3

import sys
import json
import copy 
from fuzzywuzzy import fuzz

THRESHOLD = 80

if len(sys.argv) != 3:
   print("2 args needed: <source json file> <db json file>")
   sys.exit(1)


with open(sys.argv[1]) as source_json_file:
   with open(sys.argv[2]) as db_json_file:
      source = json.load(source_json_file)
      db = json.load(db_json_file)
      if not "soundSection" in source:
         sys.exit()
      for engine in source["soundSection"]["engines"]:
         for parameter in engine["parameters"]:
            if not "component" in parameter:
               max_ratio_key = ""
               max_ratio_value = 0
               for key, names in db["parameters"].items():
                  max_ratio_for_key = 0
                  for name in names:
                     ratio = fuzz.token_set_ratio(name.lower(), parameter["name"].lower())
                     max_ratio_for_key = max(ratio, max_ratio_for_key)
                  if max_ratio_for_key > max_ratio_value:
                      max_ratio_value = max_ratio_for_key
                      max_ratio_key = key
                  if max_ratio_value >= THRESHOLD:
                     component_name = ""
                     if not "components" in engine:
                        engine["components"] = []
                     for comp in engine["components"]:
                        if "role" in comp:
                           if comp["role"] == max_ratio_key
                              component_name = comp["name"]
                     if component_name == "":
                        component_name = max_ratio_key
                        engine["components"].append({ "name" : max_ratio_key, "role" : max_ratio_key })
                     parameter["component"] = component_name

            if not "role" in parameter:
               for key, names in db["parameters"].items():
                  
                  for name in names:
                     ratio = fuzz.token_sort_ratio(name.lower(), parameter["name"].lower())
                     if ratio >= THRESHOLD:
                        parameter["role"] = key
                        #print("Ratio between {} <-> {} = {} --> {}".format(name, parameter["name"], ratio, key))
      print(json.dumps(source, indent=2))