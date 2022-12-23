

|                    | DK noteOnOff                   | DK parameter                                    |
| :---               |    :----:                      |          :---:                                  |
| W - Press-Release  | wcoord --(voiceId)-> drumvoice | wcoord --(voiceId, compId, paramId)-> drumparam |
| W - Absolute       |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam |
| W - Relative       |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam |
| W - Incremental    |    X                           | wcoord --(voiceId, compId, paramId)-> drumparam |
| N - Press-Release  | note --(voiceId)-> drumvoice   | note --(voiceId, compId, paramId)-> drumparam   |
| N - Absolute       |    X                           | note --(voiceId, compId, paramId)-> drumparam   |
| N - Relative       |    X                           | note --(voiceId, compId, paramId)-> drumparam   |
| N - Incremental    |    X                           | note --(voiceId, compId, paramId)-> drumparam   |
| AW - Press-Release | wcoord -> drumvoice            |      X                                          |
| AW - Absolute      |    X                           |      X                                          |
| AW - Relative      |    X                           |      X                                          |
| AW - Incremental   |    X                           |      X                                          |
| AN - Press-Release | note -> chromatic play drumvoice |    X                                          |
| AN - Absolute      |    X                           |      X                                          |
| AN - Relative      |    X                           |      X                                          |
| AN - Incremental   |    X                           |      X                                          |



|                    | MK noteOnOff   | MK parameter                              |
| :---               |    :----:      |          :---:                            |
| W - Press-Release  | X              | wcoord --(compId, paramId)-> MKparam      |
| W - Absolute       |    X           | wcoord --(compId, paramId)-> MKparam      |
| W - Relative       |    X           | wcoord --(compId, paramId)-> MKparam      |
| W - Incremental    |    X           | wcoord --(compId, paramId)-> MKparam      |
| N - Press-Release  | X              | note --(compId, paramId)-> MKparam        |
| N - Absolute       |    X           | note --(compId, paramId)-> MKparam        |
| N - Relative       |    X           | note --(compId, paramId)-> MKparam        |
| N - Incremental    |    X           | note --(compId, paramId)-> MKparam        |
| AW - Press-Release | X              |      X                                    |
| AW - Absolute      |     X          |      X                                    |
| AW - Relative      |     X          |      X                                    |
| AW - Incremental   |     X          |      X                                    |
| AN - Press-Release | note -> MKNote |    X                                      |
| AN - Absolute      |      X         | note --(compId, paramId)-> MKparamPerNote |
| AN - Relative      |      X         | note --(compId, paramId)-> MKparamPerNote |
| AN - Incremental   |      X         | note --(compId, paramId)-> MKparamPerNote |



|                    | MD voice noteOnOff                           | MD voice parameter                  |
| :---               |    :----:                                    |          :---:                      |
| W - Press-Release  | wcoord --(voiceId)-> play-defaultpitch-voice | wcoord --(voiceId, paramId)-> param |
| W - Absolute       |    X                                         | wcoord --(voiceId, paramId)-> param |
| W - Relative       |    X                                         | wcoord --(voiceId, paramId)-> param |
| W - Incremental    |    X                                         | wcoord --(voiceId, paramId)-> param |
| N - Press-Release  | note --(voiceId)-> play-defaultpitch-voice   | note --(voiceId, paramId)-> param   |
| N - Absolute       |    X                                         | note --(voiceId, paramId)-> param   |
| N - Relative       |    X                                         | note --(voiceId, paramId)-> param   |
| N - Incremental    |    X                                         | note --(voiceId, paramId)-> param   |
| AW - Press-Release |     X                                        |      X                              |
| AW - Absolute      |     X                                        |      X                              |
| AW - Relative      |     X                                        |      X                              |
| AW - Incremental   |     X                                        |      X                              |
| AN - Press-Release | note --(voiceId)-> MDNote                    |    X                                |
| AN - Absolute      |      X                                       | X                                   |
| AN - Relative      |      X                                       | X                                   |
| AN - Incremental   |      X                                       | X                                   |