/******************************************************************************
*
* Names:    Nada Radwan Mohsen: 6806913
*           Leann Labelle: 6817322 
*           Justin Lafleur: 6766323
*                
* Course Code:   SEG 4145
* Lab Number:    4
* File name:     SEG4145_Lab_4
* Date:          March 8, 2017
*
*
*  Description
* *************
* The purpose of this lab is to learn how to connect the arduino robot to wifi
* and control its movements using a java program
******************************************************************************
*/


// Header files to include
#define TEMPSENSOR 0x68

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Wirefree.h>
#include <WifiServer.h>

// Global Variables

int motorLeftPin = 45;
int motorRightPin = 8;

int motorLeftSensor = 48;
int motorRightSensor = 49;

int sonarSensor = 22;

double pulseLeftBackward = 150; //150
double pulseLeftForward = 245; //190

double pulseRightBackward = 233;
double pulseRightForward = 138; //11.5

int boardLED = 13;
int lcdPinNumber = 18;

int reg = 0x01;

SoftwareSerial LCD = SoftwareSerial(0, lcdPinNumber);

// Calibration Variable

int timeForOneRotationClockwise = 2900;
int timeForOneRotationCounterClockwise = 3600; 

int timeToTraverseOneTileForward = 1500;  // The number times the loop must be of to traverse one tile forwards
int timeToTraverseOneTileBackward = 1500;  // The number times the loop must be of to traverse one tile backwards

double timeToMoveForwardOneCM = timeToTraverseOneTileForward/30.5;
double timeToMoveBackwardOneCM = timeToTraverseOneTileBackward/30.5;

// Macros for moving the motors of the robot

#define LEFTMOTORFORWARD() analogWrite(motorLeftPin, pulseLeftForward)
#define RIGHTMOTORFORWARD() analogWrite(motorRightPin, pulseRightForward)

#define LEFTMOTORBACKWARD() analogWrite(motorLeftPin, pulseLeftBackward)
#define RIGHTMOTORBACKWARD() analogWrite(motorRightPin, pulseRightBackward)

#define STOPLEFTMOTOR() analogWrite(motorLeftPin, 0)
#define STOPRIGHTMOTOR() analogWrite(motorRightPin, 0)


// Wifi Parameters

WIFI_PROFILE wireless_prof = {
                        /* SSID */ "stingray",
         /* WPA/WPA2 passphrase */ "12345678",
                  /* IP address */ "192.168.1.188",
                 /* subnet mask */ "255.255.255.0",
                  /* Gateway IP */ "192.168.1.1", };


// Initialize Server with IP address and port number
WifiServer server(80, PROTO_TCP);

void setup() {
  
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  pinMode(boardLED, OUTPUT);
  pinMode(lcdPinNumber, OUTPUT);

  Wire.begin();
  
  LCD.begin(9600);
  Serial.begin(9600);
  displayStrings("6806913", "6766323");
  flickerLED(1000,2);
  displayStrings("6766323", "6817322");
  flickerLED(1000,3);
  Wireless.begin(&wireless_prof);
  server.begin();
  delay(1000);
}

void loop(){
 WifiClient client = server.available();
  int count = 0;
  String command = "";
  char function;
  String amount;
  if (client) {
    // an HTTP request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c;
        int  b;
        
        while((b = client.read()) == -1){};
        c = b;
        if (c != '\n') {
          command = command + String(c);
        }
        if (c == '\n') {
          currentLineIsBlank = true;
          count = 0;
          function = command[0];
          amount = command.substring(2);
          command = "";
          switch(function) {
            case '1':
              moveForward(amount.toInt());
              break;
            case '2':
              moveBackward(amount.toInt());
              break;
            case '3':
              rotateClockwise(amount.toInt());
              break;
            case '4':
              rotateCounterClockwise(amount.toInt());
              break;
            case '5':
              readSonarSensor();
              break;
            case '6':
              readTemperatureSensor(client);
              break;
            case '7':
              Serial.println("Closing connection");
              client.stop();
            break;
            default :
              client.println("Invalid command");
            }
        } 
        else if (c != '\r'){
          currentLineIsBlank = false;
          count++;
          
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);        
  }
}
/******************************************************************************
* Name
**************
* moveForward
*
* Description
* *************
* This function is used to move the robot forward
*
* Parameters
* *************
* Name         Type    In/Out     Description
* ------      ------  ---------  ---------------
* distance    int     In           The distance in cm the robot should move forward
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveForward(int distance) {

  displayStrings("Moving", "Forward");
  LEFTMOTORFORWARD();
  RIGHTMOTORFORWARD();
  delay(distance*timeToMoveForwardOneCM);
  stopped();
}

/******************************************************************************
* Name
**************
* moveBackward
*
* Description
* *************
* This function is used to move the robot forward
*
* Parameters
* *************
* Name         Type    In/Out     Description
* ------      ------  ---------  ---------------
* distance    int     In         The distance in cm the robot should move backwards
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveBackward(int distance) {
  
  displayStrings("Moving", "Backward");
  LEFTMOTORBACKWARD();
  RIGHTMOTORBACKWARD();
  delay(distance*timeToMoveBackwardOneCM);
  stopped();
}

/******************************************************************************
* Name
**************
* rotateClockwise
*
* Description
* *************
* This function is used to make the robot rotate clockwise
*
* Parameters
* *************
* Name      Type    In/Out     Description
* ------   ------  ---------  ---------------
* angle    double     In       The angle that the robot must rotate
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void rotateClockwise(double angle) {
  
  displayStrings("Rotating", "Right");
  double stopWatch = angle / 360 * timeForOneRotationClockwise;
  analogWrite(motorLeftPin, 191.5);
  analogWrite(motorRightPin, 191.5);
  delay(stopWatch);
  stopped();
}

/******************************************************************************
* Name
**************
* rotateCounterClockwise
*
* Description
* *************
* This function is used to make the robot rotate counterclockwise
*
* Parameters
* *************
* Name      Type    In/Out     Description
* ------   ------  ---------  ---------------
* angle    double     In       The angle that the robot must rotate
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void rotateCounterClockwise(double angle) {
  
  displayStrings("Rotating", "Left");
  double stopWatch = angle / 360 * timeForOneRotationCounterClockwise;
  analogWrite(motorLeftPin, 10);
  analogWrite(motorRightPin, 10);
  delay(stopWatch);
  stopped();
}

/******************************************************************************
* Name
**************
* stopped
*
* Description
* *************
* This function is used to stop the motors of the robot
*
* Parameters: N/A
* *************
* Name      Type    In/Out     Description
* ------   ------  ---------  ---------------
* 
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void stopped() {
  
  displayStrings("Stopped", "");
  STOPLEFTMOTOR();
  STOPRIGHTMOTOR();
}

// LCD functions

/******************************************************************************
* Name
**************
* displayStrings
*
* Description
* *************
* This function displays two strings, one on the top line and the second on the bottom
*
* Parameters
* *************
* Name     Type    In/Out     Description
* ------   ------  ---------  ---------------
* top       String   In        String to be put on the top line
* bottom    String   In        String to be put on the bottom line
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

 void displayStrings(String top, String bottom){
  
  clearLcd();
  
  int topColumnCursor = int ((16 - top.length())/2);
  int bottomColumnCursor = int ((16 - bottom.length())/2);
  
  positionLcdCursor(0, topColumnCursor);
  LCD.print(top);

  positionLcdCursor(1, bottomColumnCursor);
  LCD.print(bottom);
 }

/******************************************************************************
* Name
**************
* postionLcdCursor
*
* Description
* *************
* This function is used to position LCD cursor
*
* Parameters
* *************
* Name     Type    In/Out     Description
* ------   ------  ---------  ---------------
* row       int     In        Row of cursor position
* col       int     In        Column of cursor position
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void positionLcdCursor(int row, int col) {
  
  LCD.write(0xFE);                     // put LCD in command mode
  LCD.write((col + row * 64 + 128));   // put the cursor in position
  delay(10);
}

/******************************************************************************
* Name
**************
* clearLcd
*
* Description
* *************
* This function is used to clear the LCD screen
*
* Parameters: N/A
* *************
* Name     Type    In/Out     Description
* ------   ------  ---------  ---------------
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void clearLcd() {
  
  LCD.write(0xFE);    // put LCD in command mode
  LCD.write(0x01);    // clear the LCD screen
  delay(10);
}

/******************************************************************************
* Name
**************
* LcdDisplayOn
*
* Description
* *************
* This function is used to turn the LCD display on 
*
* Parameters: N/A
* *************
* Name     Type    In/Out     Description
* ------   ------  ---------  ---------------
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void LcdDisplayOn() {
  
  LCD.write(0xFE);    // put LCD in command mode
  LCD.write(0x0C);    // turn LCD display on
}

/******************************************************************************
* Name
**************
* LcdDisplayOff
*
* Description
* *************
* This function is used to turn the LCD display off
*
* Parameters: N/A
* *************
* Name     Type    In/Out     Description
* ------   ------  ---------  ---------------
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void LcdDisplayOff() {
  
  LCD.write(0xFE);    // put LCD in command mode
  LCD.write(0x08);    // turn LCD display off
}

// LED functions

/******************************************************************************
* Name
**************
* flickerLED
*
* Description
* *************
* This function is used to clear the LCD screen
*
* Parameters:
* *************
* Name      Type    In/Out     Description
* ------    ------  ---------  ---------------
* timeDelay  int     In
* numTimes   int     In
* 
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void flickerLED(int timeDelay, int numTimes) {

  for(int i = 0; i < numTimes; i++) {
    digitalWrite(boardLED,HIGH); // Turn LED ON
    delay(timeDelay - 50);       // wait the time delay - 50ms
    digitalWrite(boardLED, LOW); // Turn LED OFF
    delay(50);                   // wait 50ms, used to illustrate flickering
  }
}

/******************************************************************************
* Name
**************
* readSonarSensor
*
* Description
* *************
* This function is used to read the distance of nearest object using sonar sensor
*
* Parameters:
* *************
* Name      Type    In/Out     Description
* ------    ------  ---------  ---------------
* 
* Returns: N/A
* *************
* Name       Type     Description
* ------     ------   ------------------
* distance   double    The distance of the nearest object  
* 
******************************************************************************/
double readSonarSensor() {
  pinMode(sonarSensor, OUTPUT);
  digitalWrite(sonarSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(sonarSensor, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonarSensor, LOW);
  pinMode(sonarSensor,INPUT);
  double duration = pulseIn(sonarSensor, HIGH);
  double distance = duration / (29 * 2);
  delay(10);
  displayStrings("Nearest Object", (String) distance + "cm");
  return distance;
  
}

/******************************************************************************
* Name
**************
* readSonarSensor
*
* Description
* *************
* This function is used to read the ambient temperature using the temperature sensor
*
* Parameters:
* *************
* Name      Type    In/Out     Description
* ------    ------  ---------  ---------------
* 
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void readTemperatureSensor(WifiClient client) {
  int reg = 0x01;
  for (int i = 0x01; i < 8; i++ ){
   Wire.beginTransmission(TEMPSENSOR);
    Wire.write(reg); // Indicate temperature value to read
    Wire.endTransmission();
    Wire.requestFrom(TEMPSENSOR, 1); // Request data
    while(Wire.available() < 1); // Wait for data
    byte temperatureData = Wire.read(); // Temp. value
    delay(50); // Delay 50 ms if values are read in a loop
    displayStrings("Temperature", (String) temperatureData + " degrees");
    delay(1000); 
    reg++;
  }  
}


