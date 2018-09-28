/*
  IASMidi V0.0.1

  This is to provide an interface to control a single Stereo Matrix on an Allen & Heath dLive mixing console using MIDI commands.

  Origional Copyright / Licence message:

	This is an initial sketch to be used as a "blueprint" to create apps which can be used with IOTappstory.com infrastructure
	Your code can be filled wherever it is marked.

	Copyright (c) [2017] [Andreas Spiess]

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

  IASBlink V1.0.0
*/

#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 2                                        // Button pin on the esp for selecting modes. D3 for the Wemos!

#include "midi.h"

#include <IOTAppStory.h>                                    // IotAppStory.com library
IOTAppStory IAS(COMPDATE, MODEBUTTON);                      // Initialize IOTAppStory

// ================================================ EXAMPLE VARS =========================================
// used in this example to blink (LEDpin) every (blinkTime) miliseconds
unsigned long blinkEntry;

// We want to be able to edit these example variables below from the wifi config manager
// Currently only char arrays are supported. (Keep in mind that html form fields always return Strings)
// Use functions like atoi() and atof() to transform the char array to integers or floats
// Use IAS.dPinConv() to convert Dpin numbers to integers (D6 > 14)

char* LEDpin    = "5";																      // The value given here is the default value and can be overwritten by values saved in configuration mode
char* blinkTime = "500";


// ================================================ SETUP ================================================
void setup() {
  IAS.preSetDeviceName("iasmidi");                         // preset deviceName this is also your MDNS responder: http://iasblink.local


  
  IAS.addField(midi_session, "MIDI Session", 32, 'L'); // reference to org variable | field label value | max char return
  IAS.addField(LEDpin, "ledpin", 2, 'P');                   // These fields are added to the config wifimanager and saved to eeprom. Updated values are returned to the original variable.
  IAS.addField(blinkTime, "Blinktime(mS)", 5, 'N');         // reference to org variable | field label value | max char return


  // You can configure callback functions that can give feedback to the app user about the current state of the application.
  // In this example we use serial print to demonstrate the call backs. But you could use leds etc.
  IAS.onModeButtonShortPress([]() {
    Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });

  IAS.onModeButtonLongPress([]() {
    Serial.println(F(" If mode button is released, I will enter in configuration mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });

  
  IAS.begin('P');                                           // Optional parameter: What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase(default) | 'L' Leave intact

  IAS.setCallHome(false);                                    // Set to true to enable calling home frequently (disabled by default)
  //IAS.setCallHomeInterval(60);                              // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
  
	
  //-------- Your Setup starts from here ---------------
	
	
  pinMode(IAS.dPinConv(LEDpin), OUTPUT);

  midi_setup();
}



// ================================================ LOOP =================================================
void loop() {
  IAS.loop();																// this routine handles the calling home on the configured itnerval as well as reaction of the Flash button. If short press: update of skethc, long press: Configuration
  midi_loop();


  //-------- Your Sketch starts from here ---------------

	
  if (millis() - blinkEntry > atoi(blinkTime)) {
    digitalWrite(String(LEDpin).toInt(), !digitalRead(String(LEDpin).toInt()));
    blinkEntry = millis();
  }
}
