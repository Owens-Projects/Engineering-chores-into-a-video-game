// Code for arduino to transmit values received from the PS4 controller via a python script

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>


RF24 radio(9, 10); // Digital Pins connected to CE and CSN
const byte address[6] = "00001";

String inputData;
char text[14];                 // data sent from script 2 char for left motor direction, 3 char left speed, 2 char right direction, 3 char right speed, 1 char for mower  to start and run, 1 char wipper snipper button, 1 char hedger button

void setup() {
  Serial.begin(9600);           //Serial from the usb port.
  radio.begin();
  radio.setDataRate(RF24_250KBPS);    //lowe data rate helps with extended range
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);        // levels are MIN = -18dBm, LOW = -12dBm, HIGH = -6dBm , MAX = 0dBm
  radio.stopListening();
}

void loop() {
  if (Serial.available() > 0){
    inputData = Serial.readStringUntil('x');
    inputData.toCharArray(text, inputData.length()+1);
    radio.write(&text, sizeof(text));
    delay(80);
  }

}