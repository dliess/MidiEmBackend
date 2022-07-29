#!/usr/bin/python3

import xml.etree.ElementTree as ET
import copy

midiKnobs = ET.parse('midiKnobs.xml')

tree = ET.parse('Valentin.xml')

song = tree.getroot()

kitChannelNr = 1
kitChannelNrMax = 12
synthChannelNr = 13
synthChannelNrMax = 16

def createMidiKnobs(channelNr):
    mr = copy.deepcopy(midiKnobs.getroot())
    for midiKnob in mr.findall("midiKnob"):
        midiKnob.attrib["channel"] = channelNr
    return mr


instruments = song.find("instruments")
for kit in instruments.findall("kit"):
    ss = kit.find("soundSources")
    for sound in ss.findall("sound"):
        if(kitChannelNr <= kitChannelNrMax):
            sound.append(createMidiKnobs(kitChannelNr))
            kitChannelNr = kitChannelNr + 1

for sound in instruments.findall("sound"):
    if(synthChannelNr <= synthChannelNrMax):
        sound.append(createMidiKnobs(synthChannelNr))
        synthChannelNr = synthChannelNr + 1

xmlstr = ET.tostring(midiKnobs, encoding='utf8', method='xml')
print(xmlstr)