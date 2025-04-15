#include <AccelStepper.h>
#include <MultiStepper.h>
#include <TouchScreen.h>

// --- Motor Pins ---
#define MOTOR_A_IN1  2
#define MOTOR_A_IN2  3
#define MOTOR_A_IN3  4
#define MOTOR_A_IN4  5

#define MOTOR_B_IN1  6
#define MOTOR_B_IN2  7
#define MOTOR_B_IN3  8
#define MOTOR_B_IN4  9

#define MOTOR_C_IN1 10
#define MOTOR_C_IN2 11
#define MOTOR_C_IN3 12
#define MOTOR_C_IN4 13

// --- Touchscreen Pins ---
#define XP A1
#define XM A2
#define YP A3
#define YM A0

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 480);  // Resistance from multimeter

// --- Touchscreen Calibration ---
#define TS_MINX 70
#define TS_MAXX 510
#define TS_MINY 470
#define TS_MAXY 930

#define SCREEN_WIDTH_MM 130
#define SCREEN_HEIGHT_MM 172

// --- Stepper Settings ---
#define STEPS_PER_REV 400
#define MAX_MOTOR_SPEED 300
#define MOTOR_ACCELERATION 100

AccelStepper stepperA(AccelStepper::HALF4WIRE, MOTOR_A_IN1, MOTOR_A_IN2, MOTOR_A_IN3, MOTOR_A_IN4);
AccelStepper stepperB(AccelStepper::HALF4WIRE, MOTOR_B_IN1, MOTOR_B_IN2, MOTOR_B_IN3, MOTOR_B_IN4);
AccelStepper stepperC(AccelStepper::HALF4WIRE, MOTOR_C_IN1, MOTOR_C_IN2, MOTOR_C_IN3, MOTOR_C_IN4);

MultiStepper steppers;

// --- Initialization State ---
bool systemReady = false;
long initialOffset[3] = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  Serial.println("Ball Balancer setup started...");

  // Motor setup
  stepperA.setMaxSpeed(MAX_MOTOR_SPEED);
  stepperB.setMaxSpeed(MAX_MOTOR_SPEED);
  stepperC.setMaxSpeed(MAX_MOTOR_SPEED);

  stepperA.setAcceleration(MOTOR_ACCELERATION);
  stepperB.setAcceleration(MOTOR_ACCELERATION);
  stepperC.setAcceleration(MOTOR_ACCELERATION);

  steppers.addStepper(stepperA);
  steppers.addStepper(stepperB);
  steppers.addStepper(stepperC);

  // Raise platform slightly
  long initialLift[3] = {100, 100, 100};
  steppers.moveTo(initialLift);
  steppers.runSpeedToPosition();

  Serial.println("Platform lifted. Tap the screen to begin tracking...");
}

void loop() {
  // Wait for user tap to start
  if (!systemReady) {
    TSPoint p = ts.getPoint();

    pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(YM, OUTPUT);

    if (p.z > 10) {
      initialOffset[0] = stepperA.currentPosition();
      initialOffset[1] = stepperB.currentPosition();
      initialOffset[2] = stepperC.currentPosition();

      systemReady = true;
      Serial.println("Touch detected. Starting position tracking...");
    }
    return;
  }

  // --- Ball Position Tracking Only ---
  int x_mm = 0;
  int y_mm = 0;

  getBallPosition(x_mm, y_mm);

  if (x_mm != 0 || y_mm != 0) {
    Serial.print("Ball X: ");
    Serial.print(x_mm);
    Serial.print(" mm | Y: ");
    Serial.print(y_mm);
    Serial.println(" mm");
  }

  delay(100);
}

// --- Ball Position Reader Function ---
void getBallPosition(int &x_mm, int &y_mm) {
  TSPoint p = ts.getPoint();

  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(YM, OUTPUT);

  if (p.z > 10) {
    x_mm = map(p.x, TS_MINX, TS_MAXX, 0, SCREEN_WIDTH_MM);
    y_mm = map(p.y, TS_MINY, TS_MAXY, 0, SCREEN_HEIGHT_MM);
  } else {
    x_mm = 0;
    y_mm = 0;
  }
}
