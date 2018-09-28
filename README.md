# ias-midi

This is a basic mix matrix level & send controller interface for an Allen & Heath dLive.

Desired feature set:

Control Matrix fader level ~ Output volume
Control Matrix send levels ~ Whats in the matrix, and their levels
This requires server/program to convert RTP-MIDI [https://en.wikipedia.org/wiki/RTP-MIDI] protocol to the dLive Midi protocol e.g. a MacMini running DAW Control [https://www.allen-heath.com/dlive-home/software/#accordian_item_869] with Apple MIDI Routing.

https://www.dropbox.com/s/wcrzcybjpby52oe/AH%20DAW%20Control.png?dl=0
https://www.dropbox.com/s/2ouc7boge4wibq0/Apple%20Midi%20Network%20Setup.png?dl=0

Ideally this will eventually be similar to this existing product:
https://www.allen-heath.com/ahproducts/ip1/
