#include <M5Unified.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Internal IR LED on GPIO 9
constexpr uint16_t kIrLedPin = 9;
constexpr uint16_t kFrequencyKhz = 38;

// Nikon shutter command (raw timings)
uint16_t kNikonShutterRaw[] = {
  1989, 27513, 408, 1569, 386, 3503, 408, 62013, 1985, 27517, 404, 1573, 382, 3506, 405
};
constexpr uint16_t kNikonShutterLen = sizeof(kNikonShutterRaw) / sizeof(kNikonShutterRaw[0]);

constexpr uint32_t kUiRefreshMs = 100;
constexpr uint32_t kBurstGapMs = 90;
constexpr uint32_t kTimerDelayMs = 2000;

IRsend irsend(kIrLedPin);

enum ShootMode {
  MODE_SINGLE = 0,
  MODE_TIMER,
  MODE_BURST,
  MODE_COUNT
};

ShootMode mode = MODE_SINGLE;
uint32_t shotsSent = 0;
uint32_t queuedShots = 0;
uint32_t nextShotAtMs = 0;
uint32_t lastUiMs = 0;

char statusLine[32] = "READY";

const char* modeText(ShootMode m) {
  switch (m) {
    case MODE_SINGLE: return "Single";
    case MODE_TIMER: return "2s Timer";
    case MODE_BURST: return "Burst x3";
    default: return "Unknown";
  }
}

void setStatus(const char* text) {
  snprintf(statusLine, sizeof(statusLine), "%s", text);
}

void queueSequence(ShootMode m) {
  uint32_t now = millis();
  switch (m) {
    case MODE_SINGLE:
      queuedShots = 1;
      nextShotAtMs = now;
      setStatus("SENDING...");
      break;
    case MODE_TIMER:
      queuedShots = 1;
      nextShotAtMs = now + kTimerDelayMs;
      setStatus("TIMER: 2s");
      break;
    case MODE_BURST:
      queuedShots = 3;
      nextShotAtMs = now;
      setStatus("BURSTING...");
      break;
    default:
      break;
  }
}

void sendShutter() {
  // Send twice for stronger compatibility with Nikon receivers.
  irsend.sendRaw(kNikonShutterRaw, kNikonShutterLen, kFrequencyKhz);
  delay(65);
  irsend.sendRaw(kNikonShutterRaw, kNikonShutterLen, kFrequencyKhz);
  shotsSent++;
}

void drawStaticUi() {
  M5.Display.fillScreen(BLACK);

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(CYAN, BLACK);
  M5.Display.setCursor(8, 8);
  M5.Display.print("Nikon Remote");

  M5.Display.drawFastHLine(0, 28, 240, DARKGREY);

  M5.Display.setTextSize(1);
  M5.Display.setTextColor(LIGHTGREY, BLACK);
  M5.Display.setCursor(8, 118);
  M5.Display.print("A: Shoot    B: Mode");
}

void drawDynamicUi() {
  int battery = M5.Power.getBatteryLevel();
  if (battery < 0) battery = 0;
  if (battery > 100) battery = 100;

  M5.Display.fillRect(0, 30, 240, 86, BLACK);

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setCursor(8, 36);
  M5.Display.printf("Mode: %s", modeText(mode));

  M5.Display.setCursor(8, 58);
  M5.Display.printf("Shots: %lu", (unsigned long)shotsSent);

  M5.Display.setCursor(8, 80);
  M5.Display.printf("Batt : %d%%", battery);

  M5.Display.setTextSize(2);
  uint16_t statusColor = queuedShots > 0 ? YELLOW : GREEN;
  M5.Display.setTextColor(statusColor, BLACK);
  M5.Display.setCursor(8, 100);
  M5.Display.printf("%s", statusLine);

  if (queuedShots > 0 && nextShotAtMs > millis()) {
    uint32_t remaining = (nextShotAtMs - millis() + 999) / 1000;
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(ORANGE, BLACK);
    M5.Display.setCursor(178, 102);
    M5.Display.printf("T-%lus", (unsigned long)remaining);
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(3);
  M5.Display.setBrightness(80);
  irsend.begin();

  drawStaticUi();
  drawDynamicUi();
}

void loop() {
  M5.update();
  uint32_t now = millis();

  if (M5.BtnA.wasPressed()) {
    queueSequence(mode);
  }

  if (M5.BtnB.wasPressed()) {
    mode = static_cast<ShootMode>((mode + 1) % MODE_COUNT);
    setStatus("MODE CHANGED");
  }

  if (queuedShots > 0 && now >= nextShotAtMs) {
    sendShutter();
    queuedShots--;
    if (queuedShots > 0) {
      nextShotAtMs = now + kBurstGapMs;
    } else {
      setStatus("IR SENT");
    }
  }

  if (now - lastUiMs >= kUiRefreshMs) {
    drawDynamicUi();
    lastUiMs = now;
  }

  delay(10);
}
