//  See library downloads for each library license.

// With FIXED SENDER configuration
#define DESTINATION_ADDL 3

#include <Arduino.h>
#include "WiFi.h"
#include "LoRa_E220.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <esp_sleep.h>
#include <Ticker.h>

#import "index7.h"  //Video feed HTML; do not remove

// ---------- esp32 pins --------------
 LoRa_E220 e220ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1

//LoRa_E220 e220ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e220 TX, esp32 TX --> e220 RX AUX M0 M1
// -------------------------------------


#define AUX GPIO_NUM_15

// Replace with your network details
const char *ssid = "yourSSID";
const char *password = "yourNetworkPassword";

WiFiClient client;

AsyncWebServer server(80);

int data;

struct Message {
 int switchState;
} message;
 
Ticker oneTick;
Ticker onceTick;

String linkAddress = "xxx.xxx.xxx.xxx:80";

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

volatile int watchdogCounter;
int totalwatchdogCounter;
int cameraPowerOff = 0;
int watchDog;

void ISRwatchdog() {

  portENTER_CRITICAL_ISR(&mux);

  watchdogCounter++;

  if (watchdogCounter >= 75) {

    watchDog = 1;
  }

  portEXIT_CRITICAL_ISR(&mux);
}

int cameraFlag;
int needAnotherCountdown = 0;

void ISRcamera() {
  batteryOff();
}

bool got_interrupt = false;
 
void interruptHandler() {
  got_interrupt = true;
}  // interruptHandler

void setup() {

  Serial.begin(9600);
  delay(500);
  
  // Startup all pins and UART
  e220ttl.begin();

  e220ttl.setMode(MODE_1_WOR_TRANSMITTER);

  Serial.println("Hi, I'm going to send WOR message!");

  // Send message
  ResponseStatus rs = e220ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, "Hello, world? WOR!");
  // Check If there is some problem of succesfully send
  Serial.println(rs.getResponseDescription());

  //  e220ttl.setMode(MODE_0_NORMAL);

  Serial.println("\n\n\nWebserver and");
  Serial.println("E220-900T30D Transceiver for turning ON Videofeed\n");

  wifi_Start();

  attachInterrupt(digitalPinToInterrupt(AUX), interruptHandler, FALLING);

  server.on("/relay", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, PSTR("text/html"), HTML7, processor7);
    data = 1;
    needAnotherCountdown = 1;
    countdownTrigger();
  });

  server.begin();

  oneTick.attach(1.0, ISRwatchdog);  //watchdog  ISR triggers every 1 second
} 

void loop() {
  // If something available
  if (e220ttl.available() > 1) {
    // read the String message
    ResponseContainer rc = e220ttl.receiveMessage();
    // Is something goes wrong print error
    if (rc.status.code != 1) {
      Serial.println(rc.status.getResponseDescription());
    } else {
      // Print the data received
      Serial.println(rc.status.getResponseDescription());
      Serial.println(rc.data);
    }
  }
    
  if (Serial.available()) {
    message.switchState = data;
	  message.switchState = Serial.parseInt();

	  // Send message
	  ResponseStatus rs = e220ttl.sendFixedMessage(0, 2, 23, &message, sizeof(Message));

	  // Check If there is some problem of succesfully send
	  Serial.println(rs.getResponseDescription());
  }  
}

String processor7(const String &var) {

  //index7.h

  if (var == F("LINK"))
    return linkAddress;

  return String();
}

void batteryOff() {
  int data = 2;
  switchOne(data);
  oneTick.detach();
}

void countdownTrigger() {
  // Perform countdown actions here
  Serial.println("\nCountdown timer triggered!\n");
  // Schedule the next countdown if needed
  if (needAnotherCountdown == 1) {
    onceTick.once(60, ISRcamera);
    int data = 1;
    switchOne(data);
    needAnotherCountdown = 0;
  }
}

void switchOne(int data) {

  if (data == 1) {
    int data = 1;
    message.switchState = data;
    Serial.println("Battery Switch is ON");
    Serial.println("ESP32 waking from Deep Sleep\n");
  }

  if (data == 2) {
    int data = 2;
    message.switchState = data;
    Serial.println("Battery power switched OFF");
    Serial.println("ESP32 going to Deep Sleep\n");
  }

	Serial.println("Hi, I'm going to send message!");

	// Send message
	ResponseStatus rs = e220ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, &message, sizeof(Message));
	// Check If there is some problem of succesfully send
	Serial.println(rs.getResponseDescription());
}

void wifi_Start() {

//Server settings
#define ip { 10, 0, 0, 27 }
#define subnet \
  { 255, 255, 255, 0 }
#define gateway \
  { 10, 0, 0, 1 }
#define dns \
  { 10, 0, 0, 1 }

  WiFi.mode(WIFI_AP_STA);

  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  // We start by connecting to WiFi Station
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  delay(1000);

  //setting the static addresses in function "wifi_Start
  IPAddress ip;
  IPAddress gateway;
  IPAddress subnet;
  IPAddress dns;

  WiFi.config(ip, gateway, subnet, dns);

  Serial.println("Web server running. Waiting for the ESP32 IP...");

  // Printing the ESP IP address
  Serial.print("Server IP:  ");
  Serial.println(WiFi.localIP());
  Serial.print("Port:  ");
  Serial.println("80");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
  Serial.println("\n");

  delay(500);

  WiFi.waitForConnectResult();

  Serial.printf("Connection result: %d\n", WiFi.waitForConnectResult());

  server.begin();


  if (WiFi.waitForConnectResult() != 3) {
    delay(3000);
    wifi_Start();
  }
}

void printParameters(struct Configuration configuration) {
	DEBUG_PRINTLN("----------------------------------------");

	DEBUG_PRINT(F("HEAD : "));  DEBUG_PRINT(configuration.COMMAND, HEX);DEBUG_PRINT(" ");DEBUG_PRINT(configuration.STARTING_ADDRESS, HEX);DEBUG_PRINT(" ");DEBUG_PRINTLN(configuration.LENGHT, HEX);
	DEBUG_PRINTLN(F(" "));
	DEBUG_PRINT(F("AddH : "));  DEBUG_PRINTLN(configuration.ADDH, HEX);
	DEBUG_PRINT(F("AddL : "));  DEBUG_PRINTLN(configuration.ADDL, HEX);
	DEBUG_PRINTLN(F(" "));
	DEBUG_PRINT(F("Chan : "));  DEBUG_PRINT(configuration.CHAN, DEC); DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.getChannelDescription());
	DEBUG_PRINTLN(F(" "));
	DEBUG_PRINT(F("SpeedParityBit     : "));  DEBUG_PRINT(configuration.SPED.uartParity, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getUARTParityDescription());
	DEBUG_PRINT(F("SpeedUARTDatte     : "));  DEBUG_PRINT(configuration.SPED.uartBaudRate, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getUARTBaudRateDescription());
	DEBUG_PRINT(F("SpeedAirDataRate   : "));  DEBUG_PRINT(configuration.SPED.airDataRate, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.SPED.getAirDataRateDescription());
	DEBUG_PRINTLN(F(" "));
	DEBUG_PRINT(F("OptionSubPacketSett: "));  DEBUG_PRINT(configuration.OPTION.subPacketSetting, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getSubPacketSetting());
	DEBUG_PRINT(F("OptionTranPower    : "));  DEBUG_PRINT(configuration.OPTION.transmissionPower, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getTransmissionPowerDescription());
	DEBUG_PRINT(F("OptionRSSIAmbientNo: "));  DEBUG_PRINT(configuration.OPTION.RSSIAmbientNoise, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.OPTION.getRSSIAmbientNoiseEnable());
	DEBUG_PRINTLN(F(" "));
	DEBUG_PRINT(F("TransModeWORPeriod : "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.WORPeriod, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	DEBUG_PRINT(F("TransModeEnableLBT : "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.enableLBT, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	DEBUG_PRINT(F("TransModeEnableRSSI: "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.enableRSSI, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	DEBUG_PRINT(F("TransModeFixedTrans: "));  DEBUG_PRINT(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);DEBUG_PRINT(" -> "); DEBUG_PRINTLN(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	DEBUG_PRINTLN("----------------------------------------");
}

void printModuleInformation(struct ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	DEBUG_PRINT(F("HEAD: "));  DEBUG_PRINT(moduleInformation.COMMAND, HEX);DEBUG_PRINT(" ");DEBUG_PRINT(moduleInformation.STARTING_ADDRESS, HEX);DEBUG_PRINT(" ");DEBUG_PRINTLN(moduleInformation.LENGHT, DEC);

	Serial.print(F("Model no.: "));  Serial.println(moduleInformation.model, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}
