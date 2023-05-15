#ifndef BASE_INSTRUMENTS_NOTEPLAYER_H
#define BASE_INSTRUMENTS_NOTEPLAYER_H

namespace base::instruments
{
class INotePlayer
{
public:
   virtual ~INotePlayer() = default;
   virtual void noteOn(int note, float velocity, void* token) const = 0;
   virtual void noteOff(int note, float velocity, void* token) const = 0;
};

}   // namespace base::instruments
#endif