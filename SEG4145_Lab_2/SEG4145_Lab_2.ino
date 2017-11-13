/******************************************************************************
*
* Names:    Nada Radwan Mohsen: 6806913
*           Leann Labelle: 6817322 
*           Justin Lafleur: 6766323
*                
* Course Code:   SEG 4145
* Lab Number:    2
* File name:     SEG4145_Lab_2
* Date:          February 1, 2017
*
*
*  Description
* *************
*
* 
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

double pulseLeftBackward = 10;
double pulseLeftForward = 190;


double pulseRightBackward = 191.5;
double pulseRightForward = 11.5;

int boardLED = 13;
int lcdPinNumber = 18;

int stoppedMotor = -1; //Determine which motor is stopped, -1 mean neither, 1 is the left motor and 2 is the right motor
int motion = 0; //Determine which if the robot should move forward (1) or backward (2)
SoftwareSerial LCD = SoftwareSerial(0, lcdPinNumber);

// Calibration Variable

int timeForOneRotationClockwise = 2800;
int timeForOneRotationCounterClockwise = 3550;

int transitionsToTraverseOneTileForward = 115; // The number of transitions (0 -> 1 and 1 -> 0 needed to traverse one tile forward
int transitionsToTraverseOneTileBackward = 110; // The number of transitions (0 -> 1 and 1 -> 0 needed to traverse one tile backward


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
  
  delay(2000);
  displayStrings("6806913", "6766323");
  flickerLED(1000,2);
  displayStrings("6766323", "6817322");
  flickerLED(1000,3);
  pathForward();
  stopped();
  delay(5000);
  pathBackward();
  stopped();
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
  motion = 1;
  calibrateMotion(numTiles*transitionsToTraverseOneTileForward);
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
  motion = 2;
  calibrateMotion(numTiles*transitionsToTraverseOneTileBackward);
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

/******************************************************************************
* Name
**************
* calibrateMotion
*
* Description
* *************
* This function is used to calibrate the motion of the robot so that it moves in the straight line
*
* Parameters: N/A
* *************
* Name      Type    In/Out     Description
* ------   ------  ---------  ---------------
* pulses    int     In        The number of pulses that the must be sent to the robot to ensure it moves the correct distance 
* 
* Returns: N/A
* *************
* Name     Type     Description
* ------  ------    ------------------
*
******************************************************************************/

void calibrateMotion(int pulses) {

  int rightCounter = 0;
  int leftCounter = 0;
  
  boolean oldRight = NULL;
  boolean oldLeft = NULL;
  boolean right = NULL;
  boolean left = NULL;
  
  while(leftCounter < pulses && rightCounter < pulses){

    // Get information about pulse for left motor sensor
    oldLeft = left;
    left = digitalRead(motorLeftSensor);
    
    // Get information about pulse for right sensor
    oldRight = right;
    right = digitalRead(motorRightSensor);
   
     if(oldLeft != left){
        leftCounter++;
      }
  
      if(oldRight != right){
        rightCounter++;
      }

     // Check to see if the counters are equal, if not, stop the motor of the faster wheel
     if(rightCounter + 2 < leftCounter) {
        STOPLEFTMOTOR();
        stoppedMotor = 0;
      
     } 
     
     if (rightCounter > leftCounter + 2) {
       
       STOPRIGHTMOTOR();
       stoppedMotor = 1;
       
     }
     if (stoppedMotor > -1 && rightCounter == leftCounter) {
       resumeMotion();
    }
  }
}

/******************************************************************************
* Name
**************
* resumeMotion
*
* Description
* *************
* This function is used determine which motor must be turned on to resume moving in the desired direction
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
void resumeMotion() {
  
  if (stoppedMotor == 0 && motion == 1) {
    LEFTMOTORFOWARD();
    
  } else if (stoppedMotor == 0 && motion == 2) {
    
    LEFTMOTORBACKWARD();
  }
  
  if (stoppedMotor == 1 && motion == 1) {
    
    RIGHTMOTORFOWARD();
  } else if (stoppedMotor == 1 && motion == 2) {
    
    RIGHTMOTORBACKWARD();
  }
  
  stoppedMotor = -1;
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
* pathForward
*
* Description
* *************
* This function is used to make the robot perform the steps for the path
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

void pathForward() {
  
  displayStrings("Path", "Forward");
  delay(3000);

  moveForward(2);
  rotateClockwise(90);
  moveForward(2);
  rotateClockwise(90);
  moveForward(3);
  rotateClockwise(90);
  moveForward(4);
  rotateClockwise(90);
  moveForward(2);
  rotateCounterClockwise(45);
  moveForward(1);
  rotateCounterClockwise(135);
  moveForward(4);
  rotateCounterClockwise(90);
  moveForward(2);
  rotateCounterClockwise(90);
  moveForward(2);
  stopped();
}

// Functions for given paths

/******************************************************************************
* Name
**************
* pathBackward
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

void pathBackward() {
  
  displayStrings("Path", "Backward");
  delay(3000);

  moveBackward(2);
  rotateClockwise(90);
  moveBackward(2);
  rotateClockwise(90);
  moveBackward(4);
  rotateClockwise(135);
  moveBackward(1);
  rotateClockwise(45);
  moveBackward(2);
  rotateCounterClockwise(90);
  moveBackward(4);
  rotateCounterClockwise(90);
  moveBackward(3);
  rotateCounterClockwise(90);
  moveBackward(2);
  rotateCounterClockwise(90);
  moveBackward(2);
  stopped();
}
