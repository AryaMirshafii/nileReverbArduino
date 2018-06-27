#include "SoftwareSerial.h"
#include <LiquidCrystal.h>
//Continuation of bluetoothTest, but it will play 
//Hotel California on my phone when a button is pressed on the circuit.
SoftwareSerial mySerial(10,11); //RX, TX
const int buttonPin = 9;
int buttonHighLow = 0; 
String currentItem;
//String command = "_play sharp dressed woman_";    //Testing LCS algorithm in app. Supposed to play Sharp Dressed MAN by ZZ top.
String command = "_play some foreigner_";
String serialRead = "";

int buttonValue;

LiquidCrystal lcd(0, 1, 2, 3, 4, 5);
void setup() {
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  
  Serial.begin(9600); //Initialize normal serial 
  mySerial.begin(9600); //Can be changed on AT command mode
  pinMode(buttonPin, INPUT);    // Initialize pin 2 as button pin
  //lcd.print("hello, world!");
}

void loop() {
//  buttonHighLow = digitalRead(buttonPin);
//  
//  mySerial.println("nothing");
//  delay(800);
//  if (buttonHighLow == HIGH) {
//    Serial.println(sizeof(command));
//    mySerial.println(command);
//    delay(800);
//  }else {
//    Serial.println("Not playing anything");
//  }
//
//
//  if( mySerial.available()){
//    Serial.println("Data being read is");
//    serialRead += char(mySerial.read());
//    
//  }
//  if(serialRead.length() > 0){
//    Serial.println("The data read is " + serialRead);
//  }
//
//
//  buttonValue = analogRead(A2);
//  Serial.println("Button Value is " + String(buttonValue));
//
//  
//  //lcd.clear();

  lcd.print("  CIRCUIT DIGEST");//print name

  lcd.setCursor(0, 1); // set the cursor to column 0, line 2
  delay(1000);
  //lcd.clear();
  
}



