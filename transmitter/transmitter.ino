/********************************************
 * Wireless Transmitter (transmitter.ino)
 * Created: 2014-3-22
 * Copyright 2014-2015 Ethan Smith
 *
 * Setup:
 * See `Pin Definitions`
 *
 * Description:
 * This transmits the current status of the
 * door and the voltage of the battery.
 *
 * Notes:
 * This uses the RocketScreamLowPower lib
 *
 ********************************************/

// Transmit ID Config
const char *idCode = "ident";


// Include
#include "LowPower.h" // Low power functions library
#include <VirtualWire.h>


// Pin Definitions
const int switchPin = 3;  // Digital
const int interruptPin = 1;  // 1 = Digital pin 3, 0 = Digital pin 2
const int txPin = 4;  // Digital
const int txVCCPin = 5;  // Digital
const int statusLedPin = 13;  // Digital
const int batteryPin = 3;  // Analog Pin
const int moniterEnablePin = 14;  // Analog 0


// Power Moniter Vars
const float referenceVolts = 5.0;
const float resistorFactor = 510.0;  // Factor  = 255 / (R2/(R1 + R2));


void setup() {
  DDRD &= B00000011;  // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
  DDRB = B00000000;  // set Arduino pins 8 to 15 as inputs
  PORTD |= B00000000;  // disable pullups on pins 2 to 7
  PORTB |= B00000000;  // disable pullups on pins 8 to 13

  pinMode(txVCCPin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);     // Bits per sec
  vw_set_tx_pin(txPin);

  //Serial.begin(9600);

  pinMode(interruptPin, INPUT);
  digitalWrite(interruptPin, HIGH);
}


void loop() {
  transmit();
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
}


void transmit() {
  sendMessage(getBatteryVoltage(), (digitalRead(switchPin) == HIGH ? 0 : 1));
  delay(200);
  sendMessage(getBatteryVoltage(), (digitalRead(switchPin) == HIGH ? 0 : 1));
}


void sendMessage(float voltage, int state) {
  char volts[5] = "";
  dtostrf(voltage, 4, 2, volts);

  char msg[13];
  sprintf(msg, "%s-%s-%i", idCode, volts, state);
  msg[12] = '\0';

  digitalWrite(txVCCPin, HIGH);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(txVCCPin, LOW);

  //Serial.println(msg);

  digitalWrite(statusLedPin, HIGH);
  delay(25);
  digitalWrite(statusLedPin, LOW);
}


float getBatteryVoltage() {
  // Enable battery monitoring
  pinMode(moniterEnablePin, OUTPUT);
  digitalWrite(moniterEnablePin, LOW);

  int val = analogRead(batteryPin);  // read the value from the sensor
  float volts = (val / resistorFactor) * referenceVolts;  // calculate the ratio

  // Disable battery monitoring
  pinMode(moniterEnablePin, INPUT);

  return volts;
}
