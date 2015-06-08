/********************************************
 * Mock Wireless Receiver (mock_receiver.ino)
 * Created: 2015-6-5
 * Copyright 2015 Ethan Smith
 *
 * Description:
 * This simulates the messages from the actual
 * receiver.
 *
 ********************************************/

void setup() {
   Serial.begin(9600);
}


void loop() {
   // Send closed status
   Serial.println("ident-0.00-1");
   delay(1000);

   // Send open status
   Serial.println("ident-0.00-0");
   delay(1000);
}
