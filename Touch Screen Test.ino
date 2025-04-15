#include <TouchScreen.h>

// Pin definitions based on your wiring
#define YP A3  // Y+ = Pin 2
#define XM A2  // X− = Pin 3
#define YM A0  // Y− = Pin 4
#define XP A1  // X+ = Pin 1

// Initialize touchscreen with approximate resistance (only affects pressure reading)
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 480);  // 480Ω is fine unless you have exact value

void setup() {
  Serial.begin(9600);
  Serial.println("Touchscreen test starting...");
}

void loop() {
  TSPoint p = ts.getPoint();  // Read X, Y, and pressure

  // Optionally reset pin modes (important if you're using a display on same pins)
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(YM, OUTPUT);

  // Accept ALL touches – even very light ones
  if (p.z > 0) {
    Serial.print("Raw X: ");
    Serial.print(p.x);
    Serial.print(" | Raw Y: ");
    Serial.print(p.y);
    Serial.print(" | Pressure: ");
    Serial.println(p.z);
  }

  delay(100);  // Adjust as needed
}
