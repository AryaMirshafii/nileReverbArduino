#include "SoftwareSerial.h"
//Continuation of bluetoothTest, but it will play 
//Hotel California on my phone when a button is pressed on the circuit.
SoftwareSerial mySerial(10,11); //RX, TX
const int buttonPin = 2;
int buttonHighLow = 0; 
String currentItem;
String command = "_play sharp dressed woman _";    //Testing LCS algorithm in app. Supposed to play Sharp Dressed MAN by ZZ top.
void setup() {
  
  
  
  Serial.begin(9600); //Initialize normal serial 
  mySerial.begin(9600); //Can be changed on AT command mode
  pinMode(buttonPin, INPUT);    // Initialize pin 2 as button pin
}

void loop() {
  buttonHighLow = digitalRead(buttonPin);
  mySerial.println("nothing");
  delay(800);
  if (buttonHighLow == HIGH) {
    Serial.println(sizeof(command));
    mySerial.println(command);
    delay(800);
  }else {
    Serial.println("Not playing anything");
  }

  
  
  
  
  
}



