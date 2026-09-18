**IMU value read**


\#include \<Wire.h\>

\#include \<MPU6050\_light.h\>


MPU6050 mpu(Wire);

unsigned long lastPrintTime = 0;


void setup() \{

  Serial.begin(115200);

  Wire.begin(21, 22);  // SDA=21, SCL=22 for ESP32

  

  Serial.println("Initializing MPU6050...");

  byte status = mpu.begin();

  if (status != 0) \{

    Serial.print("Error: ");

    Serial.println(status);

    while(1) delay(1000);

  \}

  

  Serial.println("Calibrating... Keep sensor flat and still");

  delay(2000);

  mpu.calcOffsets(true, true);

  Serial.println("Ready!");

  Serial.println("Tilt the sensor: Left, Right, Up, Back");

  Serial.println("Direction will print every 2 seconds");

  delay(1000);

\}


void loop() \{

  mpu.update();

  

  // Print direction every 2000 milliseconds (2 seconds)

  if (millis() - lastPrintTime \>= 2000) \{

    lastPrintTime = millis();

    

    float pitch = mpu.getAngleX();  // Up/Back

    float roll  = mpu.getAngleY();  // Left/Right

    

    // Find the dominant tilt direction

    if (abs(roll) \> abs(pitch) && abs(roll) \> 12) \{

      // Left or Right

      if (roll \< 0) \{

        Serial.println("LEFT");

      \} else \{

        Serial.println("RIGHT");

      \}

    \}

    else if (abs(pitch) \> 12) \{

      // Up or Back

      if (pitch \< 0) \{

        Serial.println("UP");

      \} else \{

        Serial.println("BACK");

      \}

    \}

    else \{

      // Optional: Print "LEVEL" when not tilted (remove this line if you don't want it)

      // Serial.println("LEVEL");

    \}

  \}

\}


