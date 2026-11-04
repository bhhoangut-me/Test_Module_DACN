
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  // Khoi tao I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("VL53L0X DISTANCE TEST");

  // Khoi tao cam bien
  if (!lox.begin(0x29, false, &Wire)) {
    Serial.println("VL53L0X NOT FOUND!");
    while (1) {
      delay(100);
    }
  }

  Serial.println("VL53L0X READY");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  }
  else {
    Serial.println("Out of range!");
  }

  delay(200);
}
