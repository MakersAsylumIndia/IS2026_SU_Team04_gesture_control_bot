**Camera feed code**


\#include "esp\_camera.h"

\#include \<WiFi.h\>


// ===========================

// Camera Pin Definitions for AI-Thinker ESP32-CAM

// ===========================

\#define PWDN\_GPIO\_NUM     32

\#define RESET\_GPIO\_NUM    -1

\#define XCLK\_GPIO\_NUM      0

\#define SIOD\_GPIO\_NUM     26

\#define SIOC\_GPIO\_NUM     27

\#define Y9\_GPIO\_NUM       35

\#define Y8\_GPIO\_NUM       34

\#define Y7\_GPIO\_NUM       39

\#define Y6\_GPIO\_NUM       36

\#define Y5\_GPIO\_NUM       21

\#define Y4\_GPIO\_NUM       19

\#define Y3\_GPIO\_NUM       18

\#define Y2\_GPIO\_NUM        5

\#define VSYNC\_GPIO\_NUM    25

\#define HREF\_GPIO\_NUM     23

\#define PCLK\_GPIO\_NUM     22


// ===========================

// Wi-Fi Access Point Credentials

// ===========================

const char\* ssid = "ESP32-CAM-Live";

const char\* password = "12345678";


// ===========================

// Web Server Setup on Port 80

// ===========================

WiFiServer server(80);


void setup() \{

  Serial.begin(115200);

  Serial.setDebugOutput(true);

  Serial.println();


  // Configure and initialize the camera

  camera\_config\_t config;

  config.ledc\_channel = LEDC\_CHANNEL\_0;

  config.ledc\_timer = LEDC\_TIMER\_0;

  config.pin\_d0 = Y2\_GPIO\_NUM;

  config.pin\_d1 = Y3\_GPIO\_NUM;

  config.pin\_d2 = Y4\_GPIO\_NUM;

  config.pin\_d3 = Y5\_GPIO\_NUM;

  config.pin\_d4 = Y6\_GPIO\_NUM;

  config.pin\_d5 = Y7\_GPIO\_NUM;

  config.pin\_d6 = Y8\_GPIO\_NUM;

  config.pin\_d7 = Y9\_GPIO\_NUM;

  config.pin\_xclk = XCLK\_GPIO\_NUM;

  config.pin\_pclk = PCLK\_GPIO\_NUM;

  config.pin\_vsync = VSYNC\_GPIO\_NUM;

  config.pin\_href = HREF\_GPIO\_NUM;

  config.pin\_sscb\_sda = SIOD\_GPIO\_NUM;

  config.pin\_sscb\_scl = SIOC\_GPIO\_NUM;

  config.pin\_pwdn = PWDN\_GPIO\_NUM;

  config.pin\_reset = RESET\_GPIO\_NUM;

  config.xclk\_freq\_hz = 20000000;

  config.pixel\_format = PIXFORMAT\_JPEG;

  

  // Select frame size (higher = better quality but slower)

  config.frame\_size = FRAMESIZE\_QVGA; // 320x240

  config.jpeg\_quality = 12;           // 0-63, lower is higher quality

  config.fb\_count = 1;                // Number of frame buffers to use


  // Initialize the camera

  esp\_err\_t err = esp\_camera\_init(&config);

  if (err != ESP\_OK) \{

    Serial.printf("Camera init failed with error 0x%x", err);

    return;

  \}


  // Start Access Point mode

  WiFi.mode(WIFI\_AP);

  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");

  Serial.print("Connect to Wi-Fi: ");

  Serial.println(ssid);

  Serial.print("Then open: http://");

  Serial.println(WiFi.softAPIP());


  // Start the web server

  server.begin();

\}


void loop() \{

  // Check for a new client connecting to the server

  WiFiClient client = server.available();

  if (!client) \{

    return;

  \}


  // Wait for data from the client to arrive

  Serial.println("New Client Connected.");

  while (client.connected() && !client.available()) \{

    delay(1);

  \}


  // Read the first line of the HTTP request

  String request = client.readStringUntil('\\r');

  Serial.println(request);

  client.flush();


  // The client is requesting the MJPEG stream

  if (request.indexOf("/stream") != -1) \{

    // Send the correct HTTP headers for a video stream

    client.println("HTTP/1.1 200 OK");

    client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");

    client.println("Connection: close");

    client.println();


    // Continuously send JPEG frames

    while (client.connected()) \{

      camera\_fb\_t \* fb = esp\_camera\_fb\_get();

      if (!fb) \{

        Serial.println("Frame capture failed");

        break;

      \}


      client.printf("--frame\\r\\n");

      client.printf("Content-Type: image/jpeg\\r\\n");

      client.printf("Content-Length: %d\\r\\n\\r\\n", fb-\>len);

      client.write(fb-\>buf, fb-\>len);

      client.printf("\\r\\n");


      esp\_camera\_fb\_return(fb);

    \}

  \}

  // If the request is for the root page ("/"), send the HTML interface

  else \{

    // Send a simple HTML page

    client.println("HTTP/1.1 200 OK");

    client.println("Content-Type: text/html");

    client.println("Connection: close");

    client.println();


    client.println("\<!DOCTYPE html\>");

    client.println("\<html\>");

    client.println("\<head\>\<title\>ESP32-CAM Live Stream\</title\>\</head\>");

    client.println("\<body style='text-align:center;'\>");

    client.println("\<h1\>ESP32-CAM Live Feed\</h1\>");

    client.println("\<img src='/stream' style='max-width:100%; height:auto;'\>");

    client.println("\</body\>");

    client.println("\</html\>");

  \}

  // Give the client time to receive the data

  delay(1);

  Serial.println("Client Disconnected.");

\}

