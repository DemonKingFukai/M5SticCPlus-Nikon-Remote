// Nikon IR Remote emulator
// Requires: M5Unified library (v0.2.7 or newer), IRremoteESP8266 (v2.8.6)
// Needs IRrecv.cpp to be deleted from IRremoteESP8266
#include <M5Unified.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Internal IR LED on GPIO 9
constexpr uint16_t kIrLedPin = 9;
IRsend irsend(kIrLedPin);

// Raw IR data for Nikon "Take Photo"
uint16_t irData[] = {
  1989, 27513, 408, 1569, 386, 3503, 408, 62013, 1985, 27517, 404, 1573, 382, 3506, 405
};
constexpr uint16_t irDataLen = sizeof(irData) / sizeof(irData[0]);
constexpr uint16_t kFrequency = 38; // kHz (Nikon standard)

void setup() {
  M5.begin();
  M5.Display.setRotation(3);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(GREEN, BLACK);
  M5.Display.setCursor(10, 30);
  M5.Display.print("Nikon IR Remote");
  M5.Display.setCursor(10, 70);
  M5.Display.print("Press Button to shoot");

  irsend.begin(); // Initialize IR sender
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5.Display.setCursor(10, 110);
    M5.Display.setTextColor(RED, BLACK);
    M5.Display.print("Sending IR...");

    // Send Nikon shutter code
    irsend.sendRaw(irData, irDataLen, kFrequency);

    // Turn off PWM to save battery
    ledcWrite(0, 0);   // Channel 0, duty 0%
    ledcDetach(0);     // Detach PWM from IR LED pin

    delay(200); // Short pause for Nikon camera

    M5.Display.setTextColor(GREEN, BLACK);
    M5.Display.setCursor(10, 110);
    M5.Display.print("IR Sent!       ");
  }

  delay(20);
}
