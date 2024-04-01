//E220_Transceiver_Videofeed_Receiver.ino
//William Lucid 3/11/2024 @ 17:17 EDT

	
/*
 * EBYTE LoRa E32
 * Stay in sleep mode and wait a wake up WOR message
 *
 * You must configure the address with 0 3 23 with WOR receiver enable
 * and pay attention that WOR period must be the same of sender
 *
 *
 * https://mischianti.org
 *
 * E32        ----- esp32
 * M0         ----- 19 (or 3.3v)
 * M1         ----- 21 (or GND)
 * RX         ----- TX2 (PullUP)
 * TX         ----- RX2 (PullUP)
 * AUX        ----- 15  (PullUP)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
 
// with this DESTINATION_ADDL 2 you must set
// WOR SENDER configuration to the other device and
// WOR RECEIVER to this device
#define DESTINATION_ADDL 2
 
// If you want use RSSI uncomment //#define ENABLE_RSSI true
// and use relative configuration with RSSI enabled
//#define ENABLE_RSSI true
 
#include "Arduino.h"
#include "LoRa_E220.h"
#include <WiFi.h>
 
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"

#define AUX GPIO_NUM_15

#define relayPin 17

int data;

struct Message {
 int switchState;
} message;
 
#define FPM_SLEEP_MAX_TIME           0xFFFFFFF
void callback() {
  Serial.println("Callback");
  Serial.flush();
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

RTC_DATA_ATTR int bootCount = 0;

bool interruptExecuted = false;

void IRAM_ATTR wakeUp() {
	 // Do not use Serial on interrupt callback
	 interruptExecuted = true;
	 detachInterrupt(AUX);
}

void printParameters(struct Configuration configuration);
 
// ---------- esp32 pins --------------
LoRa_E220 e220ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1
 
//LoRa_E32 e220ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e22 TX, esp32 TX --> e22 RX AUX M0 M1
// -------------------------------------
 
//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB
  }
  delay(100);

  e220ttl.begin();

  e220ttl.setMode(MODE_2_WOR_RECEIVER);

  delay(1000);
  Serial.println();
  Serial.println("Start deep sleep!");
  delay(100);
  attachInterrupt(AUX, wakeUp, FALLING);

  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  if (ESP_SLEEP_WAKEUP_EXT0 == wakeup_reason) {
      Serial.println("Waked up from external GPIO!");

      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

      gpio_hold_dis(GPIO_NUM_21);
      gpio_hold_dis(GPIO_NUM_19);

      gpio_deep_sleep_hold_dis();

      e220ttl.setMode(MODE_0_NORMAL);

      delay(1000);

      e220ttl.sendFixedMessage(0, DESTINATION_ADDL, 68, "We have waked up from message, but we can't read It!");
  }else{
      e220ttl.setMode(MODE_2_POWER_SAVING);

      delay(1000);
  }
}
 
// The loop function is called in an endless loop
void loop()
{
 
    if (e220ttl.available() > 1) {
      Serial.println("Message arrived!");
      ResponseStructContainer rsc = e220ttl.receiveMessage(sizeof(Message));
      struct Message message = *(Message*)rsc.data;

      //Serial.println(*(int*)(message.switchState));
      Serial.println(message.switchState); 
      free(rsc.data);

      // Work only with full connection
      e220ttl.setMode(MODE_0_NORMAL);

      delay(1000);

      ResponseStatus rsSend = e220ttl.sendFixedMessage(0, DESTINATION_ADDL, 68, "We have received the message!");
      // Check If there is some problem of succesfully send
      Serial.println(rsSend.getResponseDescription());

    if(interruptExecuted){
      
      Serial.println("WakeUp Callback, AUX pin go LOW and start receive message!");
    
      if (message.switchState == 1) {
        digitalWrite(relayPin, HIGH);
        Serial.println("\nBattery power switched ON");
        Serial.println("ESP32 wake from Deep Sleep\n");
      }
      
      if (message.switchState == 2) {
        digitalWrite(relayPin, LOW);
        Serial.println("\nBattery power switched OFF");
        Serial.println("ESP32 going to Deep Sleep");
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,LOW); 
        gpio_deep_sleep_hold_en();
        //Go to sleep now
        Serial.println("Going to sleep now");
        esp_deep_sleep_start();
        delay(100);
        Serial.println("This will never be printed");
      }    
    }    
  }
}

void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.COMMAND, HEX);Serial.print(" ");Serial.print(configuration.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, HEX);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));  Serial.print(configuration.OPTION.subPacketSetting, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));  Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));  Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeEnableLBT : "));  Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));  Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));  Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	Serial.println("----------------------------------------");
}

void printModuleInformation(struct ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD: "));  Serial.print(moduleInformation.COMMAND, HEX);Serial.print(" ");Serial.print(moduleInformation.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(moduleInformation.LENGHT, DEC);

	Serial.print(F("Model no.: "));  Serial.println(moduleInformation.model, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}
