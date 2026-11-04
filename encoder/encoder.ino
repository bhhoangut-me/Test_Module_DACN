
#include <Arduino.h>

#define ENC_A 34
#define ENC_B 35

volatile int32_t encoderCount = 0;
volatile uint8_t prevAB = 0;

portMUX_TYPE encoderMux = portMUX_INITIALIZER_UNLOCKED;

// Bang giai ma quadrature x4
const int8_t quadTable[16] = {
   0,  1, -1,  0,
  -1,  0,  0,  1,
   1,  0,  0, -1,
   0, -1,  1,  0
};

void IRAM_ATTR readEncoder() {
  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);

  uint8_t currentAB = (a << 1) | b;

  portENTER_CRITICAL_ISR(&encoderMux);

  uint8_t index = (prevAB << 2) | currentAB;

  encoderCount += quadTable[index];
  prevAB = currentAB;

  portEXIT_CRITICAL_ISR(&encoderMux);
}

void setup() {
  Serial.begin(115200);

  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  prevAB = (digitalRead(ENC_A) << 1)
           | digitalRead(ENC_B);

  attachInterrupt(digitalPinToInterrupt(ENC_A),
                  readEncoder, CHANGE);

  attachInterrupt(digitalPinToInterrupt(ENC_B),
                  readEncoder, CHANGE);

  Serial.println("Encoder Quadrature x4 Test");
}

void loop() {
  static int32_t lastCount = 0;
  static unsigned long lastTime = 0;

  if (millis() - lastTime >= 200) {
    lastTime = millis();

    portENTER_CRITICAL(&encoderMux);
    int32_t count = encoderCount;
    portEXIT_CRITICAL(&encoderMux);

    int32_t delta = count - lastCount;
    lastCount = count;

    Serial.print("Count: ");
    Serial.print(count);

    Serial.print(" | Delta: ");
    Serial.print(delta);

    Serial.print(" | Direction: ");

    if (delta > 0)
      Serial.println("Forward");
    else if (delta < 0)
      Serial.println("Reverse");
    else
      Serial.println("Stop");
  }
}
