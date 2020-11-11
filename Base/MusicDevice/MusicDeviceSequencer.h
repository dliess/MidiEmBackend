#ifndef MUSIC_DEVICE_SEQUENCER_H
#define MUSIC_DEVICE_SEQUENCER_H

namespace base::musicDevice
{

class Sequencer
{
public:
   void startSequencer() noexcept;
   void stopSequencer() noexcept;
   bool masked{false};
};

} // namespace base::musicDevice

#endif