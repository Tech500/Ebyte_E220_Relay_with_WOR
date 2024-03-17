# Ebyte_E220_Relay_with_WOR
Uses two Ebyte E220-900T30D radio transceivers to control a relay.  WOR is used to wake microcontroller.

Completed coding of Eybte’s E220 radio for remotely switching power to video camera by web browser request.  Asyncwebserver request turns on camera power and starts
countdown timer for a set viewing time.  When web request arrives; WOR (Wake on Radio) awakens deep sleeping ESP32,  relay for camera power energizes. Countdown 
timer expires relay de-engergizes.
