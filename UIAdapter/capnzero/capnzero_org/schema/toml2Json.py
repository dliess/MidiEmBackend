#!/usr/bin/python3

import toml
import json

parsed_toml = toml.load('Interface.toml')
print(json.dumps(parsed_toml, indent=2))
