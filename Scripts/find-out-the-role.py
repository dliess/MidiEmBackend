#!/usr/bin/python3

import sys
import json
import copy 
from fuzzywuzzy import fuzz

THRESHOLD = 80

def getComponentRole(db, parameterRole):
   for compRole, compRoleData in db["components"].items():
      for paramRole in compRoleData["parameters"]:
         if parameterRole == paramRole:
            return compRole
   return "Unknown"

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
               best_match_component_role = ""
               best_match_component_ratio = 0
               for component_role, component_data in db["components"].items():
                  for component_name in component_data["names"]:
                     ratio = fuzz.token_set_ratio(component_name.lower(), parameter["name"].lower())
                     if ratio > best_match_component_ratio:
                        best_match_component_ratio = ratio
                        best_match_component_role = component_role
               if best_match_component_ratio < THRESHOLD:
                  best_match_component_role = "Unknown"
               component_name = ""
               if not "components" in engine:
                  engine["components"] = []
               for comp in engine["components"]:
                  if "role" in comp:
                     if comp["role"] == best_match_component_role
                        component_name = comp["name"]
               if component_name == "":
                  component_name = best_match_component_role
                  engine["components"].append({ "name" : best_match_component_role, "role" : best_match_component_role })
               parameter["component"] = component_name

            if not "role" in parameter:
               for parameter_role, parameter_names in db["components"][getComponentRole(db, parameter["component"])]["parameters"].items():
                  for parameter_name in parameter_names:
                     ratio = fuzz.token_sort_ratio(parameter_name.lower(), parameter["name"].lower())
                     if ratio >= THRESHOLD:
                        parameter["role"] = parameter_role
                        #print("Ratio between {} <-> {} = {} --> {}".format(name, parameter["name"], ratio, parameter_role))
      print(json.dumps(source, indent=2))