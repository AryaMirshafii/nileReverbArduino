
#include <LiquidCrystal.h>
#include "SoftwareSerial.h"






//Initialize a software serial
SoftwareSerial mySerial(12, 13); //RX, TX


//Ints
int button1HighLow = 0;      // The value of button 1 either High or low
int button2HighLow = 0;      // The value of button 2 either High or low
int button3HighLow = 0;      // The value of button 3 either High or low
int button4HighLow = 0;      // The value of button 4 either High or low
int currentTemp = 0;



//INitializing the LCD
LiquidCrystal lcd(0, 1, 8, 9, 10, 11); //Initialize LCD


//Button Pins
const int button1Pin = 7;
const int button2Pin = 6;
const int button3Pin = 5;
const int button4Pin = 4;

//
String currentItem;
String command = "";    //The command to be excecuted by the system.

String bluetoothInput = "";


// Booleans
boolean isPlayingMusic = false;
boolean isAsking = false;
boolean isTexting = false;





//Choices that the user can be asked
String choices[3];




boolean isReading = false;
// The main setup function
void setup() {


  lcd.begin(16, 2);
  Serial.begin(9600); //Initialize normal serial
  mySerial.begin(9600); //Can be changed on AT command mode


  pinMode(button1Pin, INPUT);    // Initialize pin 7 as button pin
  pinMode(button2Pin, INPUT);    // Initialize pin 6 as button pin
  pinMode(button3Pin, INPUT);    // Initialize pin 5 as button pin
  pinMode(button4Pin, INPUT);    // Initialize pin 4 as button pin

}




// The main loop function
void loop() {
  
  //mySerial.println("Arya");
  
  if (Serial.available() && !isReading) {
    command = Serial.readString();
      command.toLowerCase();
      Serial.println(command);
      Serial.println("String length is: " + String(command.length()));
      


  } else {
    //Serial.println("isreading is " + String(isReading));
  }


  if (command != "" && command[command.length() - 1] == '\n') {
    Serial.println("Is texting is " + String(isTexting));
    if(!isTexting){
      Serial.println("Right here");
      execute(command);
    } else {
       command = "text message " + command;
       execute(command);
       isTexting = false;
    }
    
  } 



  readButtons();


  screen();               //The function that keeps the screen running
  checkInputBTData();     //Checking for communication from phone to ardunio.

}











// A function that checks for questions from the bluetooth serial.
void checkInputBTData() {

  if (mySerial.available()) {
    isReading = true;
    bluetoothInput = mySerial.readString();
    //bluetoothInput +=  char(mySerial.read());
    Serial.println(bluetoothInput);
  }else {
    isReading = false;
    bluetoothInput = "";
  }
  
  int lastIndex = bluetoothInput.length() - 1;
  if ((bluetoothInput.length() > 3) && (bluetoothInput[0] == '_' && bluetoothInput[lastIndex] == '_')) {
    Serial.println("Executing command" + bluetoothInput.length());
    execute(bluetoothInput);
    bluetoothInput = "";
    isReading = false;

  }



}





//Execute a command from the Nile Reverb
void execute(String theCommand) {
  if (theCommand[0] != '_') {

    theCommand = '_' + theCommand;
  }

  if (!isReading) {
    if (theCommand[theCommand.length() - 1] == '\n') {
      theCommand[theCommand.length() - 1] =  '_';
    }
  }



  Serial.println("The command is " + theCommand);
  if (theCommand.indexOf("go Next") > 0 || theCommand.indexOf("go Forward") > 0 || theCommand.indexOf("play Next") > 0) {
    mySerial.println(theCommand);
    
    Serial.println("Skipping forward");

  } else if (theCommand.indexOf("go Previous") > 0 || theCommand.indexOf("go Back") > 0 || theCommand.indexOf("play Previous") > 0) {
    mySerial.println(theCommand);
    Serial.println("Skipping back");

  } else if (theCommand.indexOf("Playing") > 0) {
    Serial.println("Song playing has been started");
    writeToScreen(theCommand, "");
  } else if (theCommand.indexOf("play") > 0) {
    mySerial.println(theCommand);
    Serial.println("playing some music");
  } else if (theCommand.indexOf("Ask ") > 0 ) {
    Serial.println("Asking a question");
    askQuestion(theCommand);

  } else if (theCommand.indexOf("UpdateW") > 0 ) {
    updateWeather(theCommand);
  } else if (theCommand.indexOf("weather") > 0 ) {

    Serial.println("Getting weather");
    mySerial.println(theCommand);
  } else if(theCommand.indexOf("pause") > 0){
    mySerial.println(theCommand);
    
    Serial.println("Pausing Player");
  }else if(theCommand.indexOf("start") > 0){
    mySerial.println(theCommand);
    
    Serial.println("Starting Player");
  }else if (theCommand.indexOf("text") > 0 ) {
    mySerial.println(theCommand);
    isTexting = true;
    Serial.println("Texting" + theCommand);
  }else if(theCommand.indexOf("text message") > 0){
    text(theCommand);
    isTexting = false;
  }else if(theCommand.indexOf("contact does not exist") > 0){
    writeToScreen(theCommand, "");
    isTexting = false;
  }

  //If the command still hasnt been executed at this point then it is probably invalid
  command = "";
  isReading = false;

}
void text(String message){
  if(message.length() > 20){
      Serial.println("The length exceeds 20");
  }
  mySerial.println(message);
  isTexting = false;
}







//A function that updates the weather and writes it to the
String city;
int commaIndex;
String tempString;
String spaces;
void updateWeather(String weatherString) {

  weatherString.replace("_", "");
  weatherString.replace("UpdateW", "");

  commaIndex = weatherString.indexOf(',');
  city = weatherString.substring(commaIndex + 1, weatherString.length());
  Serial.println("The updated weather data is" + weatherString);
  tempString = weatherString.substring(0, commaIndex);
  currentTemp = tempString.toInt();


  Serial.println("The temp is " + tempString);
  Serial.println("The city is " + city);



  if (tempString.length() % 2 == 0) {

    tempString = tempString + " ";
    // If the temperature is greater than 100 then add a space in front of it before printing
    //Char 176 is the degree symbol
    tempString = " Temp is: " + tempString + char(176) + "F ";
  } else {

    tempString = "", " Temp is: " + tempString + char(176) + "F ";
  }

  city = "In " + city;

  writeToScreen(tempString, city);





}

// Function that asks a question and prints it to the screen
void askQuestion(String question) {
  if(question.indexOf("what would you like to text") > 0){
    isTexting = true;
  }
  
  question.replace("Ask ", "");
  question.replace("_", "");
  Serial.println(question);
  writeToScreen(question, "");


  // Split string into choices using strtok

  isAsking = true;
  isReading = false;

}

// A function that determines how buttons 2- 4 will be used
// For example if the device is buttons will show back, pause, forward for songs
// else if the device is asking the user a question, buttons will be used for choices
int pauseButtonCount = 0;
void readButtons() {


  button1HighLow = digitalRead(button1Pin);


  if (button1HighLow == HIGH) {
    command = "_play wonderwall_";
    Serial.println(sizeof(command));
    mySerial.println(command);
    writeToScreen("Playing Hotel California By: The Eagles", "");
    Serial.println(sizeof("Playing Hotel California By: The Eagles"));
    command = "";
    delay(500);
  } else {

  }


  if (isAsking) {


    return;
  }

  if (isPlayingMusic) {
    button2HighLow = digitalRead(button2Pin);
    button3HighLow = digitalRead(button3Pin);
    button4HighLow = digitalRead(button4Pin);





    if (button2HighLow == HIGH) {
      //Play previous song
      command = "_Player goback_";
      Serial.println("Going back");


    } else if (button3HighLow = HIGH) {
      //Pause or unpause the player
      pauseButtonCount++;
      if (pauseButtonCount % 2 == 1) {
        command = "_Player pause_";
        Serial.println("Pausing song");
      } else if (pauseButtonCount % 2 == 0) {

        Serial.println("Resuming song");
        command = "_Player play_";

      }

    } else if (button4HighLow == HIGH) {
      //Play next song
      command = "_Player goforward";
      Serial.println("Going forward");
    }
    mySerial.println(command);
    delay(500);

  }
  





}








//A function that is called in the main loop and is the code that makes the screen continue running
void screen() {

  lcd.setCursor(0, 1); // set the cursor to column 0, line 2
  delay(200);//delay of 0.3sec

  lcd.scrollDisplayRight();//shifting data on LCD

  lcd.setCursor(0, 0);// set the cursor to column 0, line1
}



// A simple function that prints text to the lCD
void writeToScreen(String line1, String line2) {
  lcd.clear();

  if (line2 != "" && line2.length() > 0) {
    if (line2[0] == '_') {
      line2[0] = ' ';
    }

    if (line2[line2.length() - 1] == '_') {
      line2[line2.length() - 1] = ' ';
    }
    lcd.setCursor(0, 1); // set the cursor to column 0, line 2
    lcd.print(line2);
  }

  if (line1 != "" && line1.length() > 0) {
    if (line1[0] == '_') {
      line1[0] = ' ';
    }

    if (line1[line1.length() - 1] == '_') {
      line1[line1.length() - 1] = ' ';
    }
    lcd.setCursor(0, 0); // set the cursor to column 0, line 2
    lcd.print(line1);
  }






}



