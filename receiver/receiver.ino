/********************************************
 * Wireless Receiver (receiver.ino)
 * Created: 2014-3-22
 * Copyright 2014-2015 Ethan Smith
 *
 * Setup:
 * Pin 13 - Status LED
 * Pin 3 - RX module message line
 *
 * Description:
 * This acts as a bridge from the RX module to
 * the serial port on a computer.
 *
 ********************************************/

#include <VirtualWire.h>

void setup() {
   Serial.begin(9600);
   pinMode(13, OUTPUT);

   // Initialise the IO and ISR
   vw_set_ptt_inverted(true); // Required for receiver
   vw_setup(2000);    // Bits per sec
   vw_set_rx_pin(3);
   vw_rx_start();       // Start the receiver PLL running
}


void loop() {
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;

   if (vw_get_message(buf, &buflen)) { // Non-blocking
      digitalWrite(13, HIGH); // Flash a light to show received good message

      int i;
      for (i = 0; i < buflen; i++) {
         Serial.write(buf[i]);
      }

      Serial.println("");
      delay(10);
      digitalWrite(13, LOW);
   }
}
