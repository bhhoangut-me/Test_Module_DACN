
#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_PIN 18

Servo steeringServo;

void setup() {
  Serial.begin(115200);

  // PWM frequency = 50 Hz
  steeringServo.setPeriodHertz(50);

  // Pulse width range: 1000 - 2000 us
  steeringServo.attach(SERVO_PIN, 1000, 2000);

  Serial.println("MG90S SERVO TEST");
}

void loop() {

  // Angle -90 degrees
  steeringServo.writeMicroseconds(1000);
  Serial.println("Angle: -90 | Pulse: 1000 us");
  delay(1000);

  // Angle 0 degrees
  steeringServo.writeMicroseconds(1500);
  Serial.println("Angle: 0 | Pulse: 1500 us");
  delay(1000);

  // Angle +90 degrees
  steeringServo.writeMicroseconds(2000);
  Serial.println("Angle: +90 | Pulse: 2000 us");
  delay(1000);
}
