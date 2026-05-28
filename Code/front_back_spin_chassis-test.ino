// =====================================
// ESP32 Tank Robot
// SAME WIRING
// FULL SPEED TEST
// =====================================

// LEFT DRIVER
#define LEFT_IA 12
#define LEFT_IB 14

// RIGHT DRIVER
#define RIGHT_IA 26
#define RIGHT_IB 27

void setup() {

  // Attach PWM to pins
  ledcAttach(LEFT_IA, 5000, 8);
  ledcAttach(LEFT_IB, 5000, 8);

  ledcAttach(RIGHT_IA, 5000, 8);
  ledcAttach(RIGHT_IB, 5000, 8);

  stopMotors();

  delay(2000);
}

void loop() {

  // =========================
  // FORWARD FULL SPEED 10s
  // =========================

  ledcWrite(LEFT_IA, 255);
  ledcWrite(LEFT_IB, 0);

  ledcWrite(RIGHT_IA, 255);
  ledcWrite(RIGHT_IB, 0);

  delay(10000);

  stopMotors();
  delay(1000);

  // =========================
  // BACKWARD FULL SPEED 10s
  // =========================

  ledcWrite(LEFT_IA, 0);
  ledcWrite(LEFT_IB, 255);

  ledcWrite(RIGHT_IA, 0);
  ledcWrite(RIGHT_IB, 255);

  delay(10000);

  stopMotors();
  delay(1000);

  // =========================
  // CLOCKWISE SPIN 5s
  // =========================

  ledcWrite(LEFT_IA, 255);
  ledcWrite(LEFT_IB, 0);

  ledcWrite(RIGHT_IA, 0);
  ledcWrite(RIGHT_IB, 255);

  delay(5000);

  stopMotors();
  delay(1000);

  // =========================
  // COUNTER CLOCKWISE 10s
  // =========================

  ledcWrite(LEFT_IA, 0);
  ledcWrite(LEFT_IB, 255);

  ledcWrite(RIGHT_IA, 255);
  ledcWrite(RIGHT_IB, 0);

  delay(10000);

  stopMotors();
  delay(1000);
}

// =====================================
// STOP FUNCTION
// =====================================

void stopMotors() {

  ledcWrite(LEFT_IA, 0);
  ledcWrite(LEFT_IB, 0);

  ledcWrite(RIGHT_IA, 0);
  ledcWrite(RIGHT_IB, 0);
}