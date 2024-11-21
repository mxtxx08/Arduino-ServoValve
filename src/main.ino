#include <Servo.h>

Servo ServoValve;

/* Connections   */
const byte Switch = 2;         //Switch pin

const byte pinServo= 8;        //Servovalve Pin

/* Variables   */

volatile bool SwitchState=LOW; //Switch state

// Make sure the servo horn is correctly positioned at 0°.
const byte posStart = 0;  //Degrees to start(Servovalve closed)
const byte posEnd = 60;   //Degrees to end  (Servovalve open)

byte Angle=0;             //Rotation angle

unsigned long previousMillis = 0;
const long waitingTime = 4000;   //Sevovalve time opened

bool waiting = false;            //Boolean variable to start the counting

void Action(){                   //Interrupt function
  SwitchState=!SwitchState;      //Change the Interrupt Variable to True
}

void setup() {
  pinMode(Switch,INPUT_PULLUP);  //PinMode as input pullup
  ServoValve.attach(pinServo);
  ServoValve.write(posStart);
  attachInterrupt(digitalPinToInterrupt(Switch), Action, CHANGE); // Interrupt settings: Call the function when voltage changes
}

void loop() {
  if (SwitchState == HIGH && !waiting){                      //If Switch State changes:
    for (Angle = posStart; Angle <= posEnd; Angle += 1) {    //Move the Servovalve to posEnd
      ServoValve.write(Angle);
      delay(15);
    }
    waiting = true;                                          //Counting start
    previousMillis = millis();
  }
    if (waiting && (millis() - previousMillis >= waitingTime)) {//Wait the waiting Time and rotate to until initial position the Servo
      for (Angle = posEnd; Angle > posStart; Angle -= 1) {
        ServoValve.write(Angle);
        delay(15);
      }
    waiting = false;                                           //Stop to count
    SwitchState = LOW;                                         //Wait another signal change
  }
}
