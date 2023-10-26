#ifndef NOTE_ON_MAP_H
#define NOTE_ON_MAP_H

#include <array>
#include <bitset>
#include <optional>
#include <memory>
#include "MusicDevice.h"
#include "Midi.h"

namespace base::midifriends
{

class NoteOnMap
{
public:
    void setNoteOn(int voiceIdx, int note);
    void setNoteOff(int voiceIdx, int note);
    template<typename Cb> void forEachNoteOn(Cb&& cb);
    void clear() noexcept;
private:
   static constexpr int MIDI_MAX_NOTE = 128;
   using Map = std::array<std::bitset<MIDI_MAX_NOTE>, midi::NUM_CHANNELS>;
   Map m_map{ 0 };
};

} // namespace base::midifriends
#include "NoteOnMap.inl"

#endif // !NOTE_ON_MAP_H
