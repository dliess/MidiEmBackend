#ifndef MUSIC_DEVICE_SOUND_HANDLER_TYPES_H
#define MUSIC_DEVICE_SOUND_HANDLER_TYPES_H

#include "Midi1Input.h"
#include "Midi1Output.h"
#include "DoubleBufferedMessageDrain.h"
#include "SoundMidiInMsgHandler.h"
#include "SoundMidiOutMsgHandler.h"

namespace base::musicDevice::sound
{

using MidiInput  = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
using MidiOutput = midi::Midi1Output;
using MidiInMsgHandlerT  = MidiInMsgHandler<std::shared_ptr<MidiInput>>;
using MidiOutMsgHandlerT = MidiOutMsgHandler<std::shared_ptr<MidiOutput>>;

}   // namespace base::musicDevice::sound

#endif