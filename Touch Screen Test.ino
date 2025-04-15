#include <TouchScreen.h>

// === Touchscreen Pin Definitions ===
#define XP A1  // X+
#define XM A2  // X-
#define YP A3  // Y+
#define YM A0  // Y-

// === Calibration (Finalized from all four corners) ===
#define TS_MINX 70
#define TS_MAXX 510
#define TS_MINY 470
#define TS_MAXY 930

// === Physical Size of the Platform (in mm) ===
#define SCREEN_WIDTH_MM 130
#define SCREEN_HEIGHT_MM 172

// === Touchscreen Object ===
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 480);  // 480Ω based on hardware

void setup() {
  Serial.begin(9600);
  Serial.println("Touchscreen test with mm mapping started...");
}

void loop() {
  TSPoint p = ts.getPoint();

  // Restore pin states (critical for clean reads)
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(YM, OUTPUT);

  // Only respond if screen is being touched
  if (p.z > 10) {
    // Map raw ADC readings to physical size
    int x_mm = map(p.x, TS_MINX, TS_MAXX, 0, SCREEN_WIDTH_MM);
    int y_mm = map(p.y, TS_MINY, TS_MAXY, 0, SCREEN_HEIGHT_MM);

    // Clamp results to physical limits
    x_mm = constrain(x_mm, 0, SCREEN_WIDTH_MM);
    y_mm = constrain(y_mm, 0, SCREEN_HEIGHT_MM);

    // Output the mapped values
    Serial.print("X (mm): ");
    Serial.print(x_mm);
    Serial.print(" | Y (mm): ");
    Serial.print(y_mm);
    Serial.print(" | Pressure: ");
    Serial.println(p.z);
  }

  delay(100);  // Slow down print rate for readability
}
