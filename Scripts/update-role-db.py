#!/usr/bin/python3

import sys
import json
import copy
from fuzzywuzzy import fuzz

roleMap = {
      "Unknown" : [],
      "NoteTrigger" : [
         "TrackVolume", 
         "TrigChance",
         "Swing", 
         "Mute", 
         "Solo", 
         "KeyPrioMode" 
      ],
      "Oscillator" : [
         "Pitch",
         "PitchFinetune",
         "OSCWaveform",
         "OSCGlide", 
         "OSCKeyboardTracking", 
         "OSCSync", ,
         "OSCSlop", 
         "OSCMix", 
         "OSCNoise", 
         "SubOSCLevel", 
         "UnisonMode", 
         "UnisonOnOff" 
      ],
      "Amp" : [
         "InitialVolume", 
         "Volume",
         "Pan",
         "DelaySend",
         "ReverbSend",
         "FeedbackVolume", 
         "FeedbackGain" 
      ],
      "Filter" : [
         "FilterCutoff",
         "FilterType",
         "FilterResonance",
         "FilterSlope",  
         "FilterKeyAmount", 
         "FilterAudioModuation" 
      ],
      "LPFilter" : [           
         "LPFilterCutoff",     
         "LPFilterResonance",  
         "LPFilterSlope",       
         "LPFilterKeyAmount", 
         "LPFilterAudioModuation" 
      ],
      "HPFilter" : [           
         "HPFilterCutoff",     
         "HPFilterResonance",  
         "HPFilterSlope",       
         "HPFilterKeyAmount", 
         "HPFilterAudioModuation" 
      ],
      "LPHPFilter" : [           
         "LPHPFilterCutoff",     
         "LPHPFilterResonance",  
         "LPHPFilterSlope",       
         "LPHPFilterKeyAmount", 
         "LPHPFilterAudioModuation" 
      ],
      "Envelope" : [
         "AmpEnvAmount",
         "EnvAmount", 
         "EnvVelAmount", 
         "EnvRepeat", 
         "EnvDelay", 
         "EnvDestination", 
         "Attack",
         "Decay",
         "Sustain",
         "Release"
      ],
      "LFO" : [
         "LFOAmount", 
         "LFOFrequency",
         "LFOMultiplier",
         "LFOWaveform",
         "LFODestination",
         "LFODepth",
         "LFOKeySync" 
      ],
      "Arpeggiator" : [
         "ArpOnOff", 
         "ArpMode" 
      ],
      "Sequencer" : [
         "SequencerOnOff", 
         "SequenceTrig" 
      ],
      "Sample" : [
         "SampleStart",
         "SampleEnd",
         "SampleLength",
         "SampleLoop",
         "SampleReverse",
         "SampleRate",
         "SampleBitReduction", 
         "SampleSlot" 
      ],
      "Effects" : [],
      "Delay": [], 
      "Reverb": [], 
      "Chorus": [], 
      "Distortion": [], 
      "Compressor" : [], 
      "ModMatrix": [
         "ModSource", 
         "ModDestination", 
         "ModAmount", 
         "PitchBendRange" 
      ], 
      "Tempo": [
         "BPM", 
         "ClockDivide" 
      ] 
}

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
         if "components" in engine: 
            for component in engine["components"]:
               if "role" in component:
                  if not component["role"] in db["components"]:
                     db["components"][component["role"]]["names"] = [component["role"]]
                  if not component["name"] in db["components"][component["role"]]["names"]:
                     db["components"][component["role"]]["names"].append(component["name"])

         for parameter in engine["parameters"]:
            if "role" in parameter:
               component_role = getComponentRole(db, parameter["role"])
               max_ratio = 0
               name_of_best_match = ""
               for name in db["components"][component_role]["names"]:
                  ratio = fuzz.token_set_ratio(name.lower(), parameter["name"].lower())
                  if ratio > max_ratio:
                     max_ratio = ratio
                     name_of_best_match = name
               new_param_name = ""
               for comp_name_part in name_of_best_match.split():
                  for param_name_part in parameter["name"].split():
                     ratio = fuzz.ratio(comp_name_part.lower(), param_name_part.lower())
                     if ratio < 90:
                        if new_param_name != "":
                            new_param_name += " "
                        new_param_name += param_name_part
               
               if not parameter["role"] in db["components"][component_role]["parameters"]:
                  db["components"][component_role]["parameters"][parameter["role"]] = [parameter["role"]]
               if not new_param_name in db["components"][component_role]["parameters"][parameter["role"]]:
                  db["components"][component_role]["parameters"][parameter["role"]].append(new_param_name)
      db_json_file_w = open(sys.argv[2], "w")
      db_json_file_w.write(json.dumps(db, indent=2))
      db_json_file_w.close()
