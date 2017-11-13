/******************************************************************************
*
* Names:    Nada Radwan Mohsen: 6806913
*           Leann Labelle: 6817322 
*           Justin Lafleur: 6766323
*                
* Course Code:   SEG 4145
* Lab Number:    3
* File name:     SEG4145_Lab_3
* Date:          February 15, 2017
*
*
*  Description
* *************
* The purpose of this lab is to learn how to use the sonar and temperature to perform various operations.
* The robot is programmed to move forward 1 tile at a time. While it is moving it uses the sonar sensor
* to detect if an object is less than or equal to 10 cm away. If an object is detected, an avoidance
* algorithm is called which involves reading the ambient temperature using the temperature sensor for a
* period of time.
*
******************************************************************************
*/


// Header files to include
#define TEMPSENSOR 0x68

#include <SoftwareSerial.h>
#include <Wire.h>

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

int boardLED = 6;
int lcdPinNumber = 18;

int reg = 0x01;

boolean readFromSensor;
SoftwareSerial LCD = SoftwareSerial(0, lcdPinNumber);

// Calibration Variable

int timeForOneRotationClockwise = 2600;
int timeForOneRotationCounterClockwise = 3300; 

int ticksToTraverseOneTileForward = 48;  // The number times the loop must be of to traverse one tile forwards
int ticksToTraverseOneTileBackward = 52;  // The number times the loop must be of to traverse one tile backwards


// Macros for moving the motors of the robot

#define LEFTMOTORFOWARD() analogWrite(motorLeftPin, pulseLeftForward)
#define RIGHTMOTORFOWARD() analogWrite(motorRightPin, pulseRightForward)

#define LEFTMOTORBACKWARD() analogWrite(motorLeftPin, pulseLeftBackward)
#define RIGHTMOTORBACKWARD() analogWrite(motorRightPin, pulseRightBackward)

#define STOPLEFTMOTOR() analogWrite(motorLeftPin, 0)
#define STOPRIGHTMOTOR() analogWrite(motorRightPin, 0)

void setup() {
  
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  pinMode(boardLED, OUTPUT);
  pinMode(lcdPinNumber, OUTPUT);

  Wire.begin();
  
  LCD.begin(9600);
  displayStrings("6806913", "6766323");
  flickerLED(1000,2);
  displayStrings("6766323", "6817322");
  flickerLED(1000,3);
}

void loop() {
  readFromSensor = true;
  moveForward(1);
  stopped();
  delay(1000);
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
* numTiles    int     In         Number of tiles the robot to must move forwards
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveForward(int numTiles) {

  displayStrings("Moving", "Forward");
  LEFTMOTORFOWARD();
  RIGHTMOTORFOWARD();
  timeMotion(numTiles*ticksToTraverseOneTileForward);
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
* numTiles    int     In         Number of tiles the robot to must move backwards
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveBackward(int numTiles) {
  
  displayStrings("Moving", "Backward");
  LEFTMOTORBACKWARD();
  RIGHTMOTORBACKWARD();
  timeMotion(numTiles*ticksToTraverseOneTileBackward);
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

/******************************************************************************
* Name
**************
* timeMotion
*
* Description
* *************
* This function is used to time the motion of the robot so that it moves in the straight line
*
* Parameters: N/A
* *************
* Name      Type    In/Out     Description
* ------   ------  ---------  ---------------
* ticks    int     In        The number of tick that the must be counted to ensure it moves the correct distance 
* 
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void timeMotion(int ticks) {

  int counter = 0;
  
  while(counter < ticks){

    if(readSonarSensor() <= 10){
        avoidObstacle(); 
        break;
    }
    counter++;
  }
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

void readTemperatureSensor() {
  Wire.beginTransmission(TEMPSENSOR);
  Wire.write(reg); // Indicate temperature value to read
  Wire.endTransmission();
  Wire.requestFrom(TEMPSENSOR, 1); // Request data
  while(Wire.available() < 1); // Wait for data
  byte temperatureData = Wire.read(); // Temp. value
  delay(50); // Delay 50 ms if values are read in a loop
  displayStrings("Temperature", (String) temperatureData + "degrees");
  flickerLED(1000,5);
}

/******************************************************************************
* Name
**************
* avoidObstacle
*
* Description
* *************
* This function is used to run the avoidance algorithm when an obstacle is detected
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

void avoidObstacle() {
  readFromSensor = false;
  stopped();
  readTemperatureSensor();
  moveBackward(1);
  rotateClockwise(90);
  moveForward(1);
  moveForward(1);
  rotateCounterClockwise(90);
}

