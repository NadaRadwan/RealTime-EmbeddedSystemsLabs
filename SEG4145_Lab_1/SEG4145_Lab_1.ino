/******************************************************************************
*
* Names:    Nada Radwan Mohsen: 6806913
*           Leann Labelle: 6817322 
*           Justin Lafleur: 6766323
*                
* Course Code:   SEG 4145
* Lab Number:    1
* File name:     SEG4145_Lab_1
* Date:          January 18, 2017
*
*
*  Description
* *************
* Learn the basics of programming a robot with an arduino board. Implement functions to make 
* robot move forward and backward as well as rotate both clockwise and counterclockwise. The functions
* will allow the robot to move in the path described in the lab manual.
* 
******************************************************************************
*/


// Header files to include

#include <SoftwareSerial.h> 

// Global Variables

int motorLeftPin = 45;
int motorRightPin = 8;

int motorLeftSensor = 48;
int motorRightSensor = 49;

double pulseLeftBackward = 6;
//double pulseLeftForward = 178.965;
double pulseLeftForward = 189.965;


double pulseRightBackward = 179;
double pulseRightForward = 8.0;

int boardLED = 13;
int lcdPinNumber = 18;

SoftwareSerial LCD = SoftwareSerial(0, lcdPinNumber);

// Calibration Variable

int timeForOneRotationClockwise = 4010;
int timeForOneRotationCounterClockwise = 4150;


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
  
  LCD.begin(9600);
}

void loop() {
  
  delay(5000);
  displayStrings("6806913", "6817322");
  flickerLED(1000,5);

  pathOne();
  stopped();
  delay(5000);
  
  pathOneReverse();
  stopped();
  delay(5000);
  
  pathTwo();
  stopped();
  delay(5000);
  
  pathTwoReverse();
  stopped();
  delay(5000);
  
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
* timeDelay    int     In       Duration for the robot to move forward
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveForward(int timeDelay) {

  displayStrings("Moving", "Forward");
  LEFTMOTORFOWARD();
  RIGHTMOTORFOWARD();
  delay(timeDelay); 
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
* timeDelay    int     In       Duration for the robot to move backward
*
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void moveBackward(int timeDelay) {
  
  displayStrings("Moving", "Backward");
  LEFTMOTORBACKWARD();
  RIGHTMOTORBACKWARD();
  delay(timeDelay);
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
  int stopWatch = angle / 360 * timeForOneRotationCounterClockwise;
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

// Functions for given paths

/******************************************************************************
* Name
**************
* pathOne
*
* Description
* *************
* This function is used to make the robot perform the step for path 1
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

void pathOne() {
  
  displayStrings("Path 1", "");
  delay(3000);

  moveForward(10000);
  rotateCounterClockwise(90);
  moveForward(10000);
  rotateClockwise(90);
  moveForward(10000);
  rotateClockwise(90);
  moveForward(10000);
  rotateCounterClockwise(90);
  moveForward(10000);
  stopped();
}

/******************************************************************************
* Name
**************
* pathTwo
*
* Description
* *************
* This function is used to make the robot perform the step for path 1 in reverse
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


void pathOneReverse() {
  displayStrings("Path 1", "Reverse");
  delay(3000);

  moveBackward(10000);
  rotateCounterClockwise(90);
  moveBackward(10000);
  rotateClockwise(90);
  moveBackward(10000);
  rotateClockwise(90);
  moveBackward(10000);
  rotateCounterClockwise(90);
  moveBackward(10000);
  stopped();
  
}

/******************************************************************************
* Name
**************
* pathTwo
*
* Description
* *************
* This function is used to make the robot perform the step for path 2
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

void pathTwo() {
  
  displayStrings("Path 2", "");
  delay(3000);
  
  moveForward(10000);
  rotateClockwise(90);
  moveForward(5000);
  rotateCounterClockwise(135);
  moveForward(10000);
  rotateClockwise(135);
  moveForward(5000);
  rotateCounterClockwise(90);
  moveForward(10000);

  stopped();
}

/******************************************************************************
* Name
**************
* pathTwoReserve
*
* Description
* *************
* This function is used to make the robot perform the step for path 2 in reverse
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


void pathTwoReverse() {
  
  displayStrings("Path 2", "Reverse");
  delay(3000);
  
  moveBackward(10000);
  rotateClockwise(90);
  moveBackward(5000);
  rotateCounterClockwise(135);
  moveBackward(10000);
  rotateClockwise(135);
  moveBackward(5000);
  rotateCounterClockwise(90);
  moveBackward(10000);
}

