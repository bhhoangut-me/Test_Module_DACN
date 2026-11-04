
#include <Arduino.h>

// MUX selection pins
#define MUX_S0 26
#define MUX_S1 25
#define MUX_S2 33

// MUX analog output
#define MUX_SIG 32

#define NUM_SENSORS 7

uint16_t sensorValue[NUM_SENSORS];

// Select MUX channel (0-7)
void selectChannel(uint8_t channel) {
  digitalWrite(MUX_S0, channel & 0x01);
  digitalWrite(MUX_S1, (channel >> 1) & 0x01);
  digitalWrite(MUX_S2, (channel >> 2) & 0x01);
}

// Read ADC from selected channel
uint16_t readSensor(uint8_t channel) {
  selectChannel(channel);

  // Allow MUX output to settle
  delayMicroseconds(50);

  // Discard first ADC conversion
  analogRead(MUX_SIG);

  uint32_t sum = 0;
  const int samples = 8;

  for (int i = 0; i < samples; i++) {
    sum += analogRead(MUX_SIG);
  }

  return sum / samples;
}

void setup() {
  Serial.begin(115200);

  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);

  pinMode(MUX_SIG, INPUT);

  analogReadResolution(12);
  analogSetPinAttenuation(MUX_SIG, ADC_11db);

  selectChannel(0);

  Serial.println("7 LINE SENSOR TEST");
}

void loop() {

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValue[i] = readSensor(i);
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("S");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorValue[i]);
    Serial.print("\t");
  }

  Serial.println();

  delay(200);
}
