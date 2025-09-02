Nikon IR Remote Emulator for M5StickC Plus

A compact IR remote emulator for Nikon cameras using the M5StickC Plus. Press the button on your device to trigger the Nikon shutter via the built-in IR LED.

⸻

Features
	•	Sends Nikon “Take Photo” IR command using the M5StickC Plus internal IR LED.
	•	Simple one-button operation.
	•	Provides visual feedback on the device’s screen when the signal is sent.
	•	Power-efficient: disables PWM after sending.

⸻

Requirements
	•	Hardware:
	•	M5StickC Plus
	•	Libraries:
	•	M5Unified v0.2.7 or newer
	•	IRremoteESP8266 v2.8.6

⚠️ Important: Move or delete IRrecv.cpp from the IRremoteESP8266 library before compiling.

⸻

Setup
	1.	Install M5Unified and IRremoteESP8266 libraries in Arduino IDE.
	2.	Remove IRrecv.cpp from the IRremoteESP8266 library folder.
	3.	Connect your M5StickC Plus and upload the code.

⸻

Usage
	1.	Power on the M5StickC Plus.
	2.	The display will show:
 			Nikon IR Remote
			Press Button to shoot
	3.	Press Button A to send the Nikon shutter IR signal.
	4.	The display updates to indicate:
 			Sending IR...
			IR Sent!
