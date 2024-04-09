#ifndef MOCK_INSTRUMENT_H
#define MOCK_INSTRUMENT_H

#include "gmock/gmock.h"
#include "Identifiable.h"
#include "Refs/InstrumentRtRef.h"

namespace base::instruments::mock
{
class MockInstrument
{
public:
    MOCK_METHOD(util::Identifiable::UUID, uuid, (), (const));
    MOCK_METHOD(void, noteOn, (int, float, void*));
    MOCK_METHOD(void, noteOff, (int, float, void*));
    MOCK_METHOD(void, onNoteOnPlayed, (detail::InstrumentRtRefConcept::NoteOnPlayedCb));
    MOCK_METHOD(void, onNoteOffPlayed, (detail::InstrumentRtRefConcept::NoteOffPlayedCb));
};

}   // namespace mock

#endif
