#include <BluetoothSerial.h> 

const int PIN_MOTOR_F1 = 14;
const int PIN_MOTOR_F2 = 27;
const int PIN_MOTOR_F3 = 26;
const int PIN_MOTOR_F4 = 25;

const int PIN_MOTOR_B1 = 34;
const int PIN_MOTOR_B2 = 35;
const int PIN_MOTOR_B3 = 32;
const int PIN_MOTOR_B4 = 33;

const int N_MOTOR_PIN = 8;
const int MOTOR_PINS[N_MOTOR_PIN] = {PIN_MOTOR_F1,PIN_MOTOR_F2, PIN_MOTOR_F3, PIN_MOTOR_F4, PIN_MOTOR_B1, PIN_MOTOR_B2, PIN_MOTOR_B3, PIN_MOTOR_B4};

void setup() {
  for(int i = 0; i < N_MOTOR_PIN; ++i) {
    const int pin = MOTOR_PINS[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void loop() {
}