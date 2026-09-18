**Mq2 gas sensor value read**


// MQ-2 Gas Sensor Reader for ESP32

// Works on standard ESP32 dev boards with ADC1 pins broken out


const int sensorPin = 34;  // Use GPIO34 (ADC1\_CH6) - an ADC1 pin


void setup() \{

  Serial.begin(115200);  // Start serial communication

  Serial.println("MQ-2 Gas Sensor Initialized");

  Serial.println("Preheating sensor... (takes about 1 minute)");

  delay(60000);  // MQ-2 needs preheating time

  Serial.println("Ready. Reading values...");

\}


void loop() \{

  int sensorValue = analogRead(sensorPin);  // Read analog value (0-4095)

  float voltage = sensorValue \* (3.3 / 4095.0);  // Convert to voltage

  

  Serial.print("Sensor Value: ");

  Serial.print(sensorValue);

  Serial.print("  |  Voltage: ");

  Serial.print(voltage);

  Serial.println(" V");

  

  // Simple threshold alert

  if (sensorValue \> 2000) \{

    Serial.println("\*\*\* GAS DETECTED! \*\*\*");

  \}

  

  delay(500);  // Read every half second

\}


