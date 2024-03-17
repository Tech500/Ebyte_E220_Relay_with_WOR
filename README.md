# Ebyte_E220_Relay_with_WOR
Uses two Ebyte E220-900T30D radio transceivers to control a relay.  WOR is used to wake E220 receiver and microcontroller.

Completed coding of Eybte’s E220 radio for remotely switching power to video camera by web browser request.  Asyncwebserver request turns on camera power and starts
countdown timer for a set viewing time.  When web request arrives; WOR (Wake on Radio) awakens E220 rreceiver and deep sleeping ESP32,  relay for camera power energizes. When countdown 
timer expires relay turns off camera power conserving battery.

Advantage of Ebyte E220-900T30D is the WOR feature; allows the sender E220 to wake up the receiving E220.  Current spec for transceiver in sleep mode is 2 uA.  Same signal used to wake up E220
receiver is used to wake micrcontroller; in this case an ESP32.

file index7.h is html page for video camera; imported by E220_Transceiver_Videofeed_Sender.ino.
