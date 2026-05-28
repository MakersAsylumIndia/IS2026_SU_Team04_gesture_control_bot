#include <Wire.h>
#include <MPU6050_light.h>
#include <esp_now.h>
#include <WiFi.h>

// =====================================
// MPU6050 Gesture Detection
// =====================================
MPU6050 mpu(Wire);
unsigned long lastGestureTime = 0;

// Gesture settings
int threshold = 12;
String currentGesture = "STOP";

// =====================================
// ESP-NOW Setup
// =====================================
// REPLACE THIS WITH YOUR RECEIVER ESP32's MAC ADDRESS
uint8_t receiverMacAddress[] = {0x78, 0x1C, 0x3C, 0xB8, 0x52, 0x2C};  // CHANGE THIS!

// Structure to send data
typedef struct struct_message {
  char command[10];
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// UNIVERSAL CALLBACK - Works with all ESP32 Core versions
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // ========== MPU6050 Setup ==========
  Wire.begin(21, 22);
  
  Serial.println("Initializing MPU6050...");
  byte status = mpu.begin();
  if (status != 0) {
    Serial.print("MPU6050 Error: ");
    Serial.println(status);
    while(1) delay(1000);
  }
  
  Serial.println("Calibrating... Keep sensor FLAT and STILL");
  delay(3000);
  mpu.calcOffsets(true, true);
  
  Serial.println("=================================");
  Serial.println("GESTURE TRANSMITTER READY!");
  Serial.println("Tilt the sensor to control the car");
  Serial.println("=================================");
  
  // ========== ESP-NOW Setup ==========
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  // Use C-style cast to avoid type errors across versions
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
  Serial.print("Transmitter MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("ESP-NOW Ready!");
}

void loop() {
  mpu.update();
  
  if (millis() - lastGestureTime >= 150) {
    lastGestureTime = millis();
    
    float pitch = mpu.getAngleX();
    float roll = mpu.getAngleY();
    
    String newCommand = "";
    
    if (abs(roll) <= threshold && abs(pitch) <= threshold) {
      newCommand = "STOP";
    }
    else if (abs(roll) > threshold) {
      if (roll < 0) newCommand = "LEFT";
      else newCommand = "RIGHT";
    }
    else if (abs(pitch) > threshold) {
      if (pitch < 0) newCommand = "FRONT";
      else newCommand = "BACK";
    }
    
    if (newCommand != currentGesture) {
      currentGesture = newCommand;
      Serial.print("Sending: ");
      Serial.println(currentGesture);
      
      currentGesture.toCharArray(myData.command, 10);
      esp_now_send(receiverMacAddress, (uint8_t *) &myData, sizeof(myData));
    }
  }
}