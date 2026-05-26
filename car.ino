#include <BluetoothSerial.h>
BluetoothSerial ESP_BT;

// 前面 L298N
const int PIN_MOTOR_F1 = 14;  // 前左 IN1
const int PIN_MOTOR_F2 = 27;  // 前左 IN2
const int PIN_MOTOR_F3 = 26;  // 前右 IN3
const int PIN_MOTOR_F4 = 25;  // 前右 IN4

// 后面 L298N
// 注意：不要用 GPIO34 / GPIO35，它们只能输入
const int PIN_MOTOR_B1 = 18;  // 后左 IN1
const int PIN_MOTOR_B2 = 19;  // 后左 IN2
const int PIN_MOTOR_B3 = 32;  // 后右 IN3
const int PIN_MOTOR_B4 = 33;  // 后右 IN4

const int N_MOTOR_PIN = 8;
const int MOTOR_PINS[N_MOTOR_PIN] = {
  PIN_MOTOR_F1, PIN_MOTOR_F2,
  PIN_MOTOR_F3, PIN_MOTOR_F4,
  PIN_MOTOR_B1, PIN_MOTOR_B2,
  PIN_MOTOR_B3, PIN_MOTOR_B4
};

void setMotor(int in1, int in2, int dir) {
  if (dir > 0) {
    // 正转
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } 
  else if (dir < 0) {
    // 反转
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } 
  else {
    // 停止
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void drive(int leftDir, int rightDir) {
  // 左侧：前左 + 后左
  setMotor(PIN_MOTOR_F1, PIN_MOTOR_F2, leftDir);
  setMotor(PIN_MOTOR_B1, PIN_MOTOR_B2, leftDir);

  // 右侧：前右 + 后右
  setMotor(PIN_MOTOR_F3, PIN_MOTOR_F4, rightDir);
  setMotor(PIN_MOTOR_B3, PIN_MOTOR_B4, rightDir);
}

void stopCar() {
  drive(0, 0);
}

void setup() {
  Serial.begin(115200);
  ESP_BT.begin("roboS");

  for (int i = 0; i < N_MOTOR_PIN; ++i) {
    pinMode(MOTOR_PINS[i], OUTPUT);
    digitalWrite(MOTOR_PINS[i], LOW);
  }

  stopCar();

  Serial.println("ESP32 car ready. Bluetooth name: roboS");
}

void loop() {
  if (ESP_BT.available()) {
    char cmd = ESP_BT.read();
    Serial.println(cmd);

    if (cmd == 'F') {
      // 前进
      drive(1, 1);
    }

    else if (cmd == 'B') {
      // 后退
      drive(-1, -1);
    }

    else if (cmd == 'L') {
      // 左转：左侧停，右侧前进
      drive(0, 1);
    }

    else if (cmd == 'R') {
      // 右转：左侧前进，右侧停
      drive(1, 0);
    }

    else if (cmd == 'A') {
      // 原地左转：左侧后退，右侧前进
      drive(-1, 1);
    }

    else if (cmd == 'D') {
      // 原地右转：左侧前进，右侧后退
      drive(1, -1);
    }

    else if (cmd == 'S') {
      // 停止
      stopCar();
    }
  }
}