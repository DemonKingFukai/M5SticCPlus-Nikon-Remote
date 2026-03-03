# Nikon Remote for M5StickC Plus

Compact IR shutter remote for Nikon cameras using the M5StickC Plus internal IR LED.

## Highlights

- Improved on-device UI with live status, mode, battery, and shot counter
- Three shoot modes:
  - `Single`
  - `2s Timer`
  - `Burst x3`
- Better Nikon compatibility by sending each shutter trigger twice
- Simple controls:
  - `BtnA`: trigger current mode
  - `BtnB`: cycle mode

## Hardware

- M5StickC Plus

## Software

- Arduino core: `esp32@2.0.17` (tested)
- Libraries:
  - `M5Unified@0.2.7`
  - `IRremoteESP8266@2.8.6`

## Build (arduino-cli example)

```bash
arduino-cli compile \
  --fqbn esp32:esp32:m5stick-c \
  ./Nikon-Remote-M5SticCPlus.ino
```

## Release Notes

- `v1.0.0`: initial public release
- `v1.1.0`: UI/UX + functionality improvements

## License

MIT License.
