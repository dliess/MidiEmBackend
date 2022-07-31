#!/usr/bin/python3

import sys
import xml.etree.ElementTree as ET
import copy

inputFile = sys.argv[1]
outputFile = "mod_" + inputFile
midiKnobs = ET.parse('midiKnobs.xml')

tree = ET.parse(inputFile)

song = tree.getroot()

kitChannelNr = 1
kitChannelNrMax = 12
synthChannelNr = 13
synthChannelNrMax = 16

def createMidiKnobs(sound, channelNr):
    el = sound.find("midiKnobs")
    if(el):
        sound.erase(el)
    mn = copy.deepcopy(midiKnobs)
    mr = mn.getroot()
    for midiKnob in mr.findall("midiKnob"):
        midiKnob.attrib["channel"] = str(channelNr - 1)
    sound.insert(0, mr)

def createKitNoteMapping(sound, channelNr):
    el = sound.find("midiInput")
    if(el):
        sound.erase(el)
    eMidiInput = ET.Element('midiInput')
    eDevice = ET.SubElement(eMidiInput, 'device')
    eMidiInput.attrib["channel"] = str(channelNr - 1)
    eMidiInput.attrib["note"] = "1"
    eDevice.attrib["port"] = "upstreamUSB"
    sound.insert(1, eMidiInput)

def createInputDeviceForNoteMapping(sound):
    el = sound.find("inputMidiDevice")
    if(el):
        sound.erase(el)
    element = ET.Element('inputMidiDevice')
    element.attrib['port'] = "upstreamUSB"
    sound.insert(1, element)

instruments = song.find("instruments")
for kit in reversed(instruments.findall("kit")):
    ss = kit.find("soundSources")
    for sound in ss.findall("sound"):
        if(kitChannelNr <= kitChannelNrMax):
            createMidiKnobs(sound, kitChannelNr)
            createKitNoteMapping(sound, kitChannelNr)
            kitChannelNr = kitChannelNr + 1

for sound in reversed(instruments.findall("sound")):
    if(synthChannelNr <= synthChannelNrMax):
        createMidiKnobs(sound, synthChannelNr)
        createInputDeviceForNoteMapping(sound)
        sound.attrib["inputMidiChannel"] = str(synthChannelNr - 1)
        synthChannelNr = synthChannelNr + 1

#xmlstr = ET.tostring(tree, encoding='utf8', method='xml')
#print(xmlstr)
tree.write(outputFile)