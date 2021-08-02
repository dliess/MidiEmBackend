#!/usr/bin/python3

import sys
import json
import copy 
from fuzzywuzzy import process

roles = { }

if len(sys.argv) != 2:
   print("Need an argument")
   sys.exit(1)

with open(sys.argv[1]) as json_file:
   data = json.load(json_file)
   print(json.dumps(data, indent=2))
