#!/usr/bin/python3

import sys
import json

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

def morph_sound_section(sound_section):
   pass

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   if "soundSection" in data:
      if "global" in data["soundSection"]:
         if "components" in data["soundSection"]["global"]:
            components = []
            for key, value in data["soundSection"]["global"]["components"].items():
               del value["parameterIds"]
               components.append( { "name" : key, **value} )
            del data["soundSection"]["global"]["components"]
            data["soundSection"]["global"]["components"] = components

         parameters = []
         for key, value in data["soundSection"]["global"]["parameters"].items():
            parameters.append( { "name" : key, **value} )
         del data["soundSection"]["global"]["parameters"]
         data["soundSection"]["global"]["parameters"] = parameters



      if "engines" in data["soundSection"]:
         for engine in data["soundSection"]["engines"]:
            if "components" in engine:
               components = []
               for key, value in engine["components"].items():
                  del value["parameterIds"]
                  components.append( { "name" : key, **value} )
               del engine["components"]
               engine["components"] = components

            parameters = []
            for key, value in engine["parameters"].items():
               parameters.append( { "name" : key, **value} )
            del engine["parameters"]
            engine["parameters"] = parameters

      #print(json.dumps(data["soundSection"], indent=2))


   print(json.dumps(data, indent=2))
