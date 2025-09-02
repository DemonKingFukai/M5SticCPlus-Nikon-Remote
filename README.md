Nikon IR Remote Emulator

A simple IR remote emulator for Nikon cameras using the M5Stack family of devices. Press a button on your M5 device to trigger the Nikon shutter via IR.

⸻

Features
	•	Emulates Nikon IR remote “Take Photo” command.
	•	Uses the internal IR LED of the M5 device.
	•	Minimal setup, ready to use with M5StickC, M5StickC Plus, or similar devices.
	•	Visual feedback on M5 display when sending IR signal.

⸻

Requirements
	•	Hardware:
	•	M5Stack device with an internal IR LED (GPIO 9)
	•	Libraries:
	•	M5Unified v0.2.7 or newer
	•	IRremoteESP8266 v2.8.6

⚠️ Important: Move or Delete IRrecv.cpp from the IRremoteESP8266 library before compiling.

⸻

Setup
	1.	Install M5Unified and IRremoteESP8266 libraries in Arduino IDE.
	2.	Remove IRrecv.cpp from the IRremoteESP8266 library folder.
	3.	Connect your M5 device and upload the code.

⸻

How It Works
	•	Press Button A on the M5 device.
	•	The device sends the raw IR signal corresponding to the Nikon “Take Photo” command at 38 kHz.
	•	The display shows feedback: 
      Press Button to shoot
      Sending IR...
      IR Sent!
