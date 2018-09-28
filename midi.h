
/*
  midi.h
*/

#ifndef midi_h
#define midi_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "AppleMidi.h"

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI);

char* midi_session = "dlive";
bool midi_is_connected = false;

void midi_send_sys_ex()
{   
  Serial.println("SysEX: Sending Stero Matrix 1 name request");
  byte data[256];

  for (int i = 0; i < sizeof(data); i++)
    data[i] = i % 10;

  data[0] =  0xF0;
  data[1] =  0x00;
  data[2] =  0x00;
  data[3] =  0x1A;
  data[4] =  0x50;
  data[5] =  0x10;
  data[6] =  0x01;
  data[7] =  0x00;
  data[8] =  0x03;
  data[9] =  0x01;
  data[10] = 0x40;
  
  data[sizeof(data) - 1] = 0xF7;

  AppleMIDI.sysEx(data, sizeof(data));
}

void OnAppleMidiDisconnected(uint32_t ssrc) {
  midi_is_connected = false; 
  Serial.println("MIDI: Disconnected");
}

void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  midi_is_connected = true;
  Serial.print("MIDI: Connected to session ");
  Serial.println(name);
  midi_send_sys_ex();
}

void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}

void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOff from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}

char getSysExStatus(const byte* data, uint16_t length)
{
  if (data[0] == 0xF0 && data[length - 1] == 0xF7)
    return 'F'; // Full SysEx Command
  else if (data[0] == 0xF0 && data[length - 1] != 0xF7)
    return 'S'; // Start of SysEx-Segment
  else if (data[0] != 0xF0 && data[length - 1] != 0xF7)
    return 'M'; // Middle of SysEx-Segment
  else
    return 'E'; // End of SysEx-Segment
}

void OnAppleMidiSysExChannelName(const byte* data, uint16_t length) {
  int channel = data[10];

  for (int i = 11; i < (length - 1); i++)
  {
    Serial.print(char(data[i]));
    Serial.print(" ");
  }  
}

bool midi_is_name_header(const byte* data, uint16_t length) {
  return data[0] == 0xF0 && data[1] == 0x0 && data[2] == 0x0 && data[3] == 0x1A && data[4] == 0x50  && data[5] == 0x10 && data[6] == 0x1 && data[7] == 0x0 && data[8] == 0x3 && data[9] == 0x2;
}

void OnAppleMidiSysEx(const byte* data, uint16_t length) {
  if (midi_is_name_header(data, length)) {
    OnAppleMidiSysExChannelName(data, length);
  }
  else {
    Serial.print(F("SYSX: ("));
    Serial.print(getSysExStatus(data, length));
    Serial.print(F(", "));
    Serial.print(length);
    Serial.print(F(" bytes) "));
    for (int i = 0; i < length; i++)
    {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void midi_setup()
{
  AppleMIDI.begin(midi_session);
  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);
  AppleMIDI.OnReceiveSysEx(OnAppleMidiSysEx);
}

void midi_loop()
{
  AppleMIDI.run();
}


#endif
