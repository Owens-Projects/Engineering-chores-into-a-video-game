// Code for arduino to recieve values from transmitter

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 10); // Digital Pins connected to CE and CSN
const byte address[6] = "00001";

const int rightMotorFWD = 2;
const int rightMotor = 3;
const int rightMotorREV = 4;
const int leftMotorFWD = 7;
const int leftMotor = 6;
const int leftMotorREV = 8;
const int motorBladeStart = 22;
const int motorBladeRun = 24;
const int WSRun = 26;
const int HRun = 28;
String RFWD;
String RREV;
String LFWD;
String LREV;
String Rval;
String Lval;
String data;
String startM;     //signal to start mower
String startWS;    // signal to start whipper snipper
String startH;     // signal to start hedger
int RintVal;
int LintVal;
int LFWDint;
int LREVint;
int RFWDint;
int RREVint;
int startMint;
int startWSint;
int startHint;
int startMLastVal = 0;
int startWSLastVal = 0;
int startHLastVal = 0;
bool motorBladeRunning = false;
bool WSRunning = false;
bool HRunning = false;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);   //added to help extend range
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);        // levels are MIN = -18dBm, LOW = -12dBm, HIGH = -6dBm , MAX = 0dBm
  radio.startListening();
  pinMode(rightMotorFWD, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  pinMode(rightMotorREV, OUTPUT);
  pinMode(leftMotorFWD, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(leftMotorREV, OUTPUT);
  pinMode(motorBladeStart, INPUT);     //need relay pins to be high impedance for Relay OFF and digital low for Relay ON
  pinMode(motorBladeRun, INPUT);
  pinMode(WSRun, INPUT);
  pinMode(HRun, INPUT);
}

void loop() {
  if (radio.available()){

    char text[32] = "";
    radio.read(&text, sizeof(text));

    data = String(text);

    LFWD = data.substring(0,1);
    LREV = data.substring(1,2);
    RFWD = data.substring(5,6);
    RREV = data.substring(6,7);
    Lval = data.substring(2,5);
    Rval = data.substring(7,10);
    startM = data.substring(10,11);
    startWS = data.substring(11,12);
    startH = data.substring(12,13);

    LFWDint = LFWD.toInt();
    LREVint = LREV.toInt();
    RFWDint = RFWD.toInt();
    RREVint = RREV.toInt();
    startMint = startM.toInt();
    startWSint = startWS.toInt();
    startHint = startH.toInt();
    LintVal = Lval.toInt();
    RintVal = Rval.toInt();

    // sets direction for left motor driver and speed
    if (LFWDint == 1 and LREVint == 0){
      digitalWrite(leftMotorREV, LOW);
      digitalWrite(leftMotorFWD, HIGH);
      analogWrite(leftMotor, LintVal);
    } 
    else if (LFWDint == 0 and LREVint == 1){
      digitalWrite(leftMotorFWD, LOW);
      digitalWrite(leftMotorREV, HIGH);
      analogWrite(leftMotor, LintVal);
    }
    else{
      digitalWrite(leftMotorFWD, LOW);
      digitalWrite(leftMotorREV, LOW);
      analogWrite(leftMotor, 0);
    }
    
    // sets direction for right motor driver and speed
    if (RFWDint == 1 and RREVint == 0){
      digitalWrite(rightMotorREV, LOW);
      digitalWrite(rightMotorFWD, HIGH);
      analogWrite(rightMotor, RintVal);
    } 
    else if (RFWDint == 0 and RREVint == 1){
      digitalWrite(rightMotorFWD, LOW);
      digitalWrite(rightMotorREV, HIGH);
      analogWrite(rightMotor, RintVal);
    }
    else{
      digitalWrite(rightMotorFWD, LOW);
      digitalWrite(rightMotorREV, LOW);
      analogWrite(rightMotor, 0);
    }
    
    // Turn mower blades on or off
    if (startMint == 1 and startMLastVal == 0){
      if (motorBladeRunning == true){
        pinMode(motorBladeRun, INPUT);                  //high impedance turns off relay
        motorBladeRunning = false;
      }
      else{
        pinMode(motorBladeStart, OUTPUT);
        delay(100);
        pinMode(motorBladeRun, OUTPUT);  
        digitalWrite(motorBladeStart, LOW);
        digitalWrite(motorBladeRun, LOW);
        delay(5000);
        pinMode(motorBladeStart, INPUT);
        motorBladeRunning = true;
      }
    }
    startMLastVal = startMint;    

    // Turns whipper snipper on or off
    if (startWSint == 1 and startWSLastVal == 0){
      if (WSRunning == true){
        pinMode(WSRun, INPUT);                        //setting pin as high impedance turns relay OFF
        WSRunning = false;
      }
      else {
        pinMode(WSRun, OUTPUT); 
        digitalWrite(WSRun, LOW);                     //setting pin as digital low turns relay ON
        WSRunning = true;
      }
    }
    startWSLastVal = startWSint;

    // Turns hedger on or off
    if (startHint == 1 and startHLastVal == 0){
      if (HRunning == true){
        pinMode(HRun, INPUT);
        HRunning = false;
      }
      else {
        pinMode(HRun, OUTPUT);
        digitalWrite(HRun, LOW);
        HRunning = true;
      }
    }
    startHLastVal = startHint;
  }
}