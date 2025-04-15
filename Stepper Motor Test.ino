//This code will verify that all motors work and complete 1 full rotations simultaneously and in proper direciton

#include <AccelStepper.h>
#include <MultiStepper.h>

// Define steppers (using HALF4WIRE mode for SN754410)
AccelStepper stepperA(AccelStepper::HALF4WIRE, 2, 3, 4, 5);    // IN1-IN4 Motor A
AccelStepper stepperB(AccelStepper::HALF4WIRE, 6, 7, 8, 9);    // Motor B
AccelStepper stepperC(AccelStepper::HALF4WIRE, A2, A3, A4, A5); // Motor C

//Object to control all three motors at once
MultiStepper steppers;

void setup() {
  Serial.begin(9600);

  // Set max speeds (tune these later)
  stepperA.setMaxSpeed(200);
  stepperB.setMaxSpeed(200);
  stepperC.setMaxSpeed(200);

  // Add all steppers to Multistepper group to command all three to target positon together
  steppers.addStepper(stepperA);
  steppers.addStepper(stepperB);
  steppers.addStepper(stepperC);

  //Full rotation test
  // Example: Move each motor 1 revolution (assuming 200 steps/rev * 2 for half-step) Where 400 = 1 full rotation
  long positions[3] = {400, 400, 400}; //Create array with target positon for each motor in an array

  //Execute move
  steppers.moveTo(positions); //Sets destination for motors
  steppers.runSpeedToPosition();  // Blocking move
}

void loop() {
  // Nothing here — just setup test
}
