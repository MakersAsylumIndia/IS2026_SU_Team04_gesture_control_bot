**Simple motor front back**  
  
// ESP32 Tank Robot with L9110 Motor Driver

// Sequence: Forward 5s → Back 5s → Right 90° → Left 90° → Repeat

// L9110 pins: B-IA, B-IB = Left motor | A-IA, A-IB = Right motor


// Left Motor (L9110 Channel B)

\#define LEFT\_IA   12   // Motor direction pin 1

\#define LEFT\_IB   14   // Motor direction pin 2


// Right Motor (L9110 Channel A)  

\#define RIGHT\_IA  26   // Motor direction pin 1

\#define RIGHT\_IB  27   // Motor direction pin 2


// Speed pins? L9110 doesn't need PWM pins - just use digital pins with delay

// But for speed control, we'll use any PWM-capable pins (here using same pins)


int motorSpeed = 200;      // PWM value 0-255 (speed for forward/back)

int turnSpeed = 180;       // Speed during turns (lower = smoother)

int turnTime = 800;        // Milliseconds for 90° turn (adjust this!)


void setup() \{

  Serial.begin(115200);

  

  pinMode(LEFT\_IA, OUTPUT);

  pinMode(LEFT\_IB, OUTPUT);

  pinMode(RIGHT\_IA, OUTPUT);

  pinMode(RIGHT\_IB, OUTPUT);

  

  // Configure PWM on all pins (ESP32 requires this for analogWrite)

  ledcSetup(0, 5000, 8);   // channel 0, 5kHz, 8-bit

  ledcAttachPin(LEFT\_IA, 0);

  ledcSetup(1, 5000, 8);

  ledcAttachPin(LEFT\_IB, 1);

  ledcSetup(2, 5000, 8);

  ledcAttachPin(RIGHT\_IA, 2);

  ledcSetup(3, 5000, 8);

  ledcAttachPin(RIGHT\_IB, 3);

  

  Serial.println("Tank Robot Ready!");

  Serial.println("Sequence: FORWARD 5s → BACK 5s → RIGHT 90° → LEFT 90°");

  delay(2000);

\}


void loop() \{

  forward(5);      // Move forward for 5 seconds

  stop(1);         // Pause 1 second

  backward(5);     // Move backward for 5 seconds

  stop(1);

  turnRight();     // Turn right 90°

  stop(1);

  turnLeft();      // Turn left 90°

  stop(1);

\}


// ========== Motion Functions (L9110) ==========


void forward(int seconds) \{

  Serial.println("→ FORWARD");

  // Left motor forward

  analogWrite(LEFT\_IA, motorSpeed);

  analogWrite(LEFT\_IB, 0);

  // Right motor forward

  analogWrite(RIGHT\_IA, motorSpeed);

  analogWrite(RIGHT\_IB, 0);

  delay(seconds \* 1000);

\}


void backward(int seconds) \{

  Serial.println("← BACKWARD");

  // Left motor backward

  analogWrite(LEFT\_IA, 0);

  analogWrite(LEFT\_IB, motorSpeed);

  // Right motor backward

  analogWrite(RIGHT\_IA, 0);

  analogWrite(RIGHT\_IB, motorSpeed);

  delay(seconds \* 1000);

\}


void turnRight() \{

  Serial.println("↻ TURN RIGHT 90°");

  // Pivot right: left motor forward, right motor backward

  analogWrite(LEFT\_IA, turnSpeed);

  analogWrite(LEFT\_IB, 0);

  analogWrite(RIGHT\_IA, 0);

  analogWrite(RIGHT\_IB, turnSpeed);

  delay(turnTime);

\}


void turnLeft() \{

  Serial.println("↺ TURN LEFT 90°");

  // Pivot left: left motor backward, right motor forward

  analogWrite(LEFT\_IA, 0);

  analogWrite(LEFT\_IB, turnSpeed);

  analogWrite(RIGHT\_IA, turnSpeed);

  analogWrite(RIGHT\_IB, 0);

  delay(turnTime);

\}


void stop(int seconds) \{

  Serial.println("■ STOP");

  analogWrite(LEFT\_IA, 0);

  analogWrite(LEFT\_IB, 0);

  analogWrite(RIGHT\_IA, 0);

  analogWrite(RIGHT\_IB, 0);

  delay(seconds \* 1000);

\}


