#include <TensorFlowLite_ESP32.h>

#include <eloquent_tensorflow32.h>
#include <exception.h>
#include <utility.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <HTTPClient.h>

#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// --------------------
// WiFi Configuration
// --------------------
const char* ssid = "GalaxyS23Ultra";
const char* password = "delhi2025";
// Replace with your server's IP/hostname and port
const char* serverURL = "http://192.168.0.159:5000/ocr";

// --------------------
// Camera Configuration (AI Thinker ESP32-CAM)
// --------------------
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  Serial.println("[Setup] Starting program...");

  // Connect to WiFi
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected! IP: " + WiFi.localIP().toString());

  // Configure camera settings
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // Lower resolution (e.g. QVGA) to save memory and bandwidth
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  Serial.println("[Camera] Initializing camera...");
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[Camera] Camera init failed with error 0x%x\n", err);
    ESP.restart();
  }
  Serial.println("[Camera] Camera initialized successfully.");
}

void loop() {
  Serial.println("[Loop] Capturing image...");
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("[Loop] Camera capture failed");
    return;
  }
  
  // Ensure JPEG format
  if (fb->format != PIXFORMAT_JPEG) {
    Serial.println("[Loop] Non-JPEG format not supported");
    esp_camera_fb_return(fb);
    return;
  }
  
  Serial.println("[Loop] Image captured. Sending to server...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL); 
    http.addHeader("Content-Type", "image/jpeg");
    
    int httpResponseCode = http.POST(fb->buf, fb->len);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("[Loop] Server response: " + response);
    } else {
      Serial.println("[Loop] Error sending POST: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("[Loop] WiFi disconnected");
  }
  
  esp_camera_fb_return(fb);
  
  // Delay between captures (adjust as needed)
  delay(1000);
}