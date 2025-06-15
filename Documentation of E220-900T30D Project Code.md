# Ebyte LoRa E220 Device Project Documentation

This document outlines the setup and usage of the Ebyte LoRa E220 device with Arduino, ESP32, or ESP8266, based on Renzo Mischianti's E220 articles and library.

### Renzo Mischianti's E220 Resources

  * **Ebyte LoRa E220 Articles**:
      [Settings and basic usage](https://www.google.com/search?q=https://www.mischianti.org/2021/03/12/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-settings-and-basic-usage/)
      * [Library](https://www.google.com/search?q=https://www.mischianti.org/2021/03/17/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-library/)
      * [Configuration](https://www.google.com/search?q=https://www.mischianti.org/2021/03/22/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-configuration/)
      * [Fixed transmission, broadcast, monitor, and RSSI](https://www.google.com/search?q=https://www.mischianti.org/2021/03/24/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-fixed-transmission-broadcast-monitor-and-rssi/)
      * [Power-saving and sending structured data](https://www.google.com/search?q=https://www.mischianti.org/2021/03/29/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-power-saving-and-sending-structured-data/)
      * [WOR microcontroller and Arduino shield](https://www.google.com/search?q=https://www.mischianti.org/2021/04/05/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-wor-microcontroller-and-arduino-shield/)
      * [WOR microcontroller and WeMos D1 shield](https://www.google.com/search?q=https://www.mischianti.org/2021/04/07/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-wor-microcontroller-and-wemos-d1-shield/)
      * [WOR microcontroller and ESP32 dev v1 shield](https://www.google.com/search?q=https://www.mischianti.org/2021/04/12/ebyte-lora-e220-device-for-arduino-esp32-or-esp8266-wor-microcontroller-and-esp32-dev-v1-shield/)
  * [Github Ebyte E220 library](https://github.com/xreef/EByte_LoRa_E220_Series_Library)
  * [Mischianti Arduino LoRa shield (Open source)](https://www.google.com/search?q=https://github.com/xreef/Mischianti_Arduino_LoRa_E220_Shield)
  * [Mischianti WeMos LoRa shield (Open source)](https://www.google.com/search?q=https://github.com/xreef/Mischianti_WeMos_D1_mini_LoRa_E220_Shield)
  * [Mischianti ESP32 DOIT DEV KIT v1 shield (Open source)](https://www.google.com/search?q=https://github.com/xreef/Mischianti_ESP32_DOIT_DEVKIT_V1_LoRa_E220_Shield)
  * [Support Forum](https://www.google.com/search?q=https://www.mischianti.org/forums/forum/ebyte-e220-lora-e22-e22-series/)

### Hardware Connections and Setup

A [Fritzing diagram for the Doit ESP32 Devkit V1](https://www.google.com/search?q=https://www.mischianti.org/images/fritzing/ESP32_E220_bb.png) is available.
A similar connection schema for ESP32 is used, but RX2 and TX2 are utilized for RX and TX due to ESP32 having 3 Serial ports by default instead of SoftwareSerial.

The image on page 2 illustrates the connection of the E32-TTL-100 (Ebyte LoRa E220 device) to an ESP32 dev kit v1 on a breadboard.

### Project Code and Configuration

The completed Ebyte E220-900T30D Project Code includes the following configuration settings:

1.  Load and run sender: "01\_setConfiguration\_WOR\_Sender.ino". This file can be found in the completed project code. This file is modified from the xReef, E220 Library example to use only ESP32 pins and WOR Sender configuration.
2.  Load and run receiver: "01\_setConfiguration\_WOR\_Receiver.ino". This file can be found in the completed project code. This file is modified from the xReef, E220 Library example to use only ESP32 pins and WOR Receiver configuration.

**Suggestion**: Label one breadboard "Sender" and the other "Receiver."

### Running the Transceiver Code

1.  Connect the breadboard labeled "Sender"; load "E220\_Transceiver\_Videofeed\_Sender.ino" into the Arduino IDE and run.
2.  Connect the breadboard labeled "Receiver"; load "E220\_Transceiver\_Videofeed\_Receiver.ino" into the Arduino IDE and run.

ESP32 Core 2.0.14 was used in the development of this project. The development of this E220 project relied heavily on Renzo Mischianti's E220 library and his articles.

### Monitoring with Arduino IDE Serial Monitors

1.  Using dual windows, open the Arduino IDE Serial Monitors. Open "Sender" and run it in one window.
2.  Open "Receiver" and run it in the other window, then open its serial monitor.
3.  Go to the static IP Address "10.0.0.27/relay". Make this window as small as possible while still being able to see the refresh icon.
4.  The "Sender" serial monitor should display web server information and connection results. The "Receiver" serial monitor should show "Starting Deep sleep".
5.  Select the refresh icon.
6.  The "Sender" will show a triggered countdown timer and battery power turned on. The countdown timer will expire in approximately two minutes.
7.  The "Receiver" should show "Battery power off and going to deep sleep". Repeat the refresh to wake the ESP32 by WOR.

**Note on Power Consumption**: No attempt was made to measure the ESP32 Devkit V1 deep sleep current; it was not optimized for the lowest current consumption and, as a development board, is not well suited for battery power.

**WOR Current Meter Readings**: Increased the separation between transceivers.

**EByte, E220 Series WOR Configuration notes**
