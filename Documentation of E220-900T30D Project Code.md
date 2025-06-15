# Documentation of E220-900T30D Project Code

## Renzo Mischianti's E220 Articles

- [Settings and Basic Usage](https://mischianti.org/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-specs-and-basic-use-1/)
- [Library](https://mischianti.org/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-library-2/)
- [Configuration](https://mischianti.org/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-configuration-3/)
- [Fixed Transmission, Broadcast, Monitor, and RSSI](https://mischianti.org/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-fixed-transmission-broadcast-monitor-and-rssi-4/)
- [Power-Saving and Sending Structured Data](https://mischianti.org/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-manage-wake-on-radio-and-sends-structured-data-5/)
- [WOR Microcontroller and Arduino Shield](https://mischianti.org/lora-e32-device-for-arduino-esp32-or-esp8266-wor-wake-on-radio-the-microcontroller-also-and-new-arduino-shield-part-6/)
- [WOR Microcontroller and WeMos D1 Shield](https://mischianti.org/lora-e32-device-for-arduino-esp32-or-esp8266-wor-wake-on-radio-microcontroller-and-new-wemos-d1-mini-shield-part-7/)
- [WOR Microcontroller and ESP32 Dev V1 Shield](https://mischianti.org/ebyte-lora-e32-device-for-arduino-esp32-or-esp8266-wor-wake-on-radio-and-new-esp32-shield-8/)

## GitHub Resources

- [Ebyte E220 Library](https://github.com/xreef/EByte_LoRa_E220_Series_Library)
- [Arduino LoRa Shield (Open Source)](https://www.pcbway.com/project/shareproject/LoRa_E32_Series_device_Arduino_shield.html)
- [WeMos LoRa Shield (Open Source)](https://www.pcbway.com/project/shareproject/LoRa_ESP32_DEV_KIT_v1_shield_for_EByte_E32_E22__RF_8km_12km_range.html?from=mischianti05)
- [ESP32 DOIT DEV KIT v1 Shield (Open Source)](https://github.com/xreef/Mischianti_ESP32_DOIT_DEVKIT_V1_LoRa_E220_Shield)

- [Support Forum](https://mischianti.org/forums/forum/mischiantis-libraries/ebyte-lora-e220-uart-devices-llcc68/)

## Hardware Connections and Setup

- [Fritzing Diagram for ESP32 Devkit V1](https://github.com/Tech500/Ebyte_E220_Relay_with_WOR/blob/main/E220-900T30D%20Fritzing%20Diagram.jpg)
- ESP32 connection schema: Uses RX2 and TX2 since ESP32 lacks SoftwareSerial.

## Configuration Settings

1. Load and run sender: `"01_setConfiguration_WOR_Sender.ino"` (found in completed project code).
2. Load and run receiver: `"01_setConfiguration_WOR_Receiver.ino"` (from xReef’s E220 Library example, modified for ESP32 pins and WOR configuration).
3. **Label and Connect Breadboards**:
   - **Sender**: Load `"E220_Transceiver_Videofeed_Sender.ino"` in Arduino IDE and run.
   - **Receiver**: Load `"E220_Transceiver_Videofeed_Receiver.ino"` in Arduino IDE and run.

## ESP32 Deep Sleep and WOR Functionality

- **ESP32 Core Version Used**: 2.0.14
- No measurements taken for deep sleep current consumption.
- WOR configuration requires increasing transceiver separation for reliable operation.

## Web Interface Testing

- Open **Sender** in Arduino IDE and monitor serial output.
- Open **Receiver** in another window and monitor serial output.
- Access Static IP: `http://10.0.0.27/relay`.
- Click refresh; observe WOR wake-up behavior.

## Additional Notes

- No attempt was made to measure ESP32 Devkit V1 deep sleep current due to development board constraints.
- WOR current meter readings confirmed expected transceiver behavior.
