#include <esp_now.h>
#include <WiFi.h>

// =====================================
// L9110 Motor Driver Pins
// =====================================
#define LEFT_IA   12
#define LEFT_IB   14
#define RIGHT_IA  26
#define RIGHT_IB  27

// Structure to receive data
typedef struct struct_message {
  char command[10];
} struct_message;

struct_message myData;
String currentCommand = "STOP";

// Callback when data is received - CORRECT for Core 3.x
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  String receivedCommand = String(myData.command);
  
  if (receivedCommand != currentCommand) {
    currentCommand = receivedCommand;
    Serial.print("Received: ");
    Serial.println(currentCommand);
    
    if (currentCommand == "FRONT") {
      digitalWrite(LEFT_IA, HIGH);
      digitalWrite(LEFT_IB, LOW);
      digitalWrite(RIGHT_IA, HIGH);
      digitalWrite(RIGHT_IB, LOW);
    }
    else if (currentCommand == "BACK") {
      digitalWrite(LEFT_IA, LOW);
      digitalWrite(LEFT_IB, HIGH);
      digitalWrite(RIGHT_IA, LOW);
      digitalWrite(RIGHT_IB, HIGH);
    }
    else if (currentCommand == "LEFT") {
      digitalWrite(LEFT_IA, LOW);
      digitalWrite(LEFT_IB, HIGH);
      digitalWrite(RIGHT_IA, HIGH);
      digitalWrite(RIGHT_IB, LOW);
    }
    else if (currentCommand == "RIGHT") {
      digitalWrite(LEFT_IA, HIGH);
      digitalWrite(LEFT_IB, LOW);
      digitalWrite(RIGHT_IA, LOW);
      digitalWrite(RIGHT_IB, HIGH);
    }
    else if (currentCommand == "STOP") {
      digitalWrite(LEFT_IA, LOW);
      digitalWrite(LEFT_IB, LOW);
      digitalWrite(RIGHT_IA, LOW);
      digitalWrite(RIGHT_IB, LOW);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // ========== Motor Setup ==========
  pinMode(LEFT_IA, OUTPUT);
  pinMode(LEFT_IB, OUTPUT);
  pinMode(RIGHT_IA, OUTPUT);
  pinMode(RIGHT_IB, OUTPUT);
  
  // Stop motors initially
  digitalWrite(LEFT_IA, LOW);
  digitalWrite(LEFT_IB, LOW);
  digitalWrite(RIGHT_IA, LOW);
  digitalWrite(RIGHT_IB, LOW);
  
  // ========== ESP-NOW Setup ==========
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("=================================");
  Serial.println("RECEIVER (TANK) READY!");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Waiting for gestures...");
  Serial.println("=================================");
}

void loop() {
  delay(10);
}