

|                    | DK noteOnOff                   | DK parameter                                    | method |
| :---               |    :----:                      |          :---:                                  | :---: |
| W - Press-Release  | wcoord --(voiceId)-> drumvoice | wcoord --(voiceId, compId, paramId)-> drumparam | handlePressRelease |
| W - Absolute       |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam | handleContinousValue |
| W - Relative       |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam | handleRelativeValue |
| W - Incremental    |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam | handleIncrement |
| N - Press-Release  | note --(voiceId)-> drumvoice   | note --(voiceId, compId, paramId)-> drumparam   | handlePressRelease |
| N - Absolute       |    X                           | note --(voiceId, compId, paramId)-> drumparam   | handleContinousValue |
| N - Relative       |    X                           | note --(voiceId, compId, paramId)-> drumparam   | handleRelativeValue |
| N - Incremental    |    X                           | note --(voiceId, compId, paramId)-> drumparam   | handleIncrement |
| AW - Press-Release | wcoord -> drumvoice            |      X                                          | handleAnyWidgetCoordPressRelease |
| AW - Absolute      |    X                           |      X                                          | X |
| AW - Relative      |    X                           |      X                                          | X |
| AW - Incremental   |    X                           |      X                                          | X |
| AN - Press-Release | note -> drum voice OR chromatic play drumvoice |    X                                          | handleAnyNotePressRelease |
| AN - Absolute      |    X                           |      X                                          | X |
| AN - Relative      |    X                           |      X                                          | X |
| AN - Incremental   |    X                           |      X                                          | X |



|                    | MK noteOnOff   | MK parameter                              | method |
| :---               |    :----:      |          :---:                            | :---: |
| W - Press-Release  | X              | wcoord --(compId, paramId)-> MKparam      | handlePressRelease |
| W - Absolute       |    X           | wcoord --(compId, paramId)-> MKparam      | handleContinousValue |
| W - Relative       |    X           | wcoord --(compId, paramId)-> MKparam      | handleRelativeValue |
| W - Incremental    |    X           | wcoord --(compId, paramId)-> MKparam      | handleIncrement |
| N - Press-Release  | X              | note --(compId, paramId)-> MKparam        | handlePressRelease |
| N - Absolute       |    X           | note --(compId, paramId)-> MKparam        | handleContinousValue |
| N - Relative       |    X           | note --(compId, paramId)-> MKparam        | handleRelativeValue |
| N - Incremental    |    X           | note --(compId, paramId)-> MKparam        | handleIncrement |
| AW - Press-Release | X              |      X                                    | handleAnyWidgetCoordPressRelease |
| AW - Absolute      |     X          |      X                                    | X |
| AW - Relative      |     X          |      X                                    | X |
| AW - Incremental   |     X          |      X                                    | X |
| AN - Press-Release | note -> MKNote |    X                                      | handleAnyNotePressRelease |
| AN - Absolute      |      X         | note --(compId, paramId)-> MKparamPerNote | sendMPEContinousValue |
| AN - Relative      |      X         | note --(compId, paramId)-> MKparamPerNote | sendMPERelativeValue |
| AN - Incremental   |      X         | note --(compId, paramId)-> MKparamPerNote | sendMPEIncrementValue |



|                    | MD voice noteOnOff                           | MD voice parameter                  | method |
| :---               |    :----:                                    |          :---:                      | :---: |
| W - Press-Release  | wcoord --(voiceId)-> play-defaultpitch-voice | wcoord --(voiceId, paramId)-> param | handlePressRelease |
| W - Absolute       |    X                                         | wcoord --(voiceId, paramId)-> param | handleContinousValue |
| W - Relative       |    X                                         | wcoord --(voiceId, paramId)-> param | handleRelativeValue |
| W - Incremental    |    X                                         | wcoord --(voiceId, paramId)-> param | handleIncrement |
| N - Press-Release  | note --(voiceId)-> play-defaultpitch-voice   | note --(voiceId, paramId)-> param   | handlePressRelease |
| N - Absolute       |    X                                         | note --(voiceId, paramId)-> param   | handleContinousValue |
| N - Relative       |    X                                         | note --(voiceId, paramId)-> param   | handleRelativeValue |
| N - Incremental    |    X                                         | note --(voiceId, paramId)-> param   | handleIncrement |
| AW - Press-Release |     X                                        |      X                              | handleAnyWidgetCoordPressRelease |
| AW - Absolute      |     X                                        |      X                              | X |
| AW - Relative      |     X                                        |      X                              | X |
| AW - Incremental   |     X                                        |      X                              | X |
| AN - Press-Release | note --(voiceId)-> MDNote                    |    X                                | handleAnyNotePressRelease |
| AN - Absolute      |      X                                       | X                                   | X |
| AN - Relative      |      X                                       | X                                   | X |
| AN - Incremental   |      X                                       | X                                   | X |