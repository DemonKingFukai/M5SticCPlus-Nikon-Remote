#include <M5StickCPlus.h>
#include <IRremote.h>

// Use built-in IR emitter
IRsend irsend;

// Built-in LED for feedback
const int ledPin = LED_BUILTIN;

// Timing
const unsigned long sendInterval = 1000; // interval between IR pulses
unsigned long lastSendTime = 0;
unsigned long buttonPressStart = 0;

// Visual feedback
const unsigned long flashDuration = 150;
bool circleVisible = false;
unsigned long circleStartTime = 0;

// Shutter modes
enum ShutterMode { NONE, HALF_PRESS, FULL_PRESS, CONTINUOUS };
ShutterMode currentMode = NONE;

// Nikon IR raw data (replace with your verified timings if needed)
uint16_t nikonIR[] = {1989, 27513, 408, 1569, 386, 3503, 408, 62013, 1985, 27517, 404, 1573, 382, 3506, 405};
unsigned int nikonIRlen = sizeof(nikonIR) / sizeof(nikonIR[0]);

void setup() {
  M5.begin();
  pinMode(ledPin, OUTPUT);
  M5.Lcd.begin();
  M5.Lcd.setTextSize(2);
  drawText();
}

void loop() {
  M5.update();
  unsigned long currentTime = millis();

  // Shutter button
  if (M5.BtnA.isPressed()) {
    if (buttonPressStart == 0) buttonPressStart = currentTime;
    unsigned long pressDuration = currentTime - buttonPressStart;

    // Determine shutter mode
    if (pressDuration >= 2000) currentMode = CONTINUOUS;
    else if (pressDuration >= 500) currentMode = FULL_PRESS;
    else currentMode = HALF_PRESS;

    // Send IR signal based on mode
    if (currentMode == FULL_PRESS || currentMode == CONTINUOUS) {
      if (currentTime - lastSendTime >= sendInterval) {
        lastSendTime = currentTime;
        sendNikonIR();
      }
    } else if (currentMode == HALF_PRESS && lastSendTime == 0) {
      lastSendTime = currentTime;
      sendNikonIR();
    }

  } else {
    buttonPressStart = 0;
    lastSendTime = 0;
    currentMode = NONE;
    digitalWrite(ledPin, LOW);
  }

  // Handle non-blocking circle feedback
  if (circleVisible && currentTime - circleStartTime >= flashDuration) {
    M5.Lcd.fillCircle(100, 100, 50, BLACK);
    circleVisible = false;
    drawText();
    digitalWrite(ledPin, LOW);
  }
}

// Send Nikon IR signal
void sendNikonIR() {
  irsend.sendRaw(nikonIR, nikonIRlen, 38000); // 38 kHz

  digitalWrite(ledPin, HIGH);
  M5.Lcd.fillCircle(100, 100, 50, RED);
  circleStartTime = millis();
  circleVisible = true;
}

// Redraw display
void drawText() {
  M5.Lcd.setCursor(50, 10);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.print("Shutter");
}
