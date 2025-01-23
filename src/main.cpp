#include <DoubleResetDetector.h>
#include <WiFiSettings.h>
#include <LittleFS.h>
#include "helpers.h"
#include "config.h"

DoubleResetDetector drd(10, 0);

void setup() {
  system_update_cpu_freq(160);
  Serial.begin(115200);
  Serial.println("booting up conesoft-web-button");
  pinMode(LED_BUILTIN, OUTPUT);
  LittleFS.begin();
  WiFi.forceSleepBegin();
  
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  WiFiSettings.hostname = "csft-btn-";
  String host = WiFiSettings.string("host", "", "Conesoft Web Devices Server");
  int port = WiFiSettings.integer("port", 0, 65535, 0, "Conesoft Web Devices Server Port");

  if (drd.detectDoubleReset()) {
    WiFiSettings.portal();
  }

  WiFiSettings.connect();
}

void handlePressed(int button);

void loop() {
  handlePressed(BUTTON1);
  handlePressed(BUTTON2);
  setLed(false);

  drd.loop();
  delay(10);
}

void makeWebRequest(int button) {
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect("192.168.1.2", 17184) && (retries-- > 0)) {
  }
  if(!!!client.connected()) {
    error(2);
  }
  client.print(String("GET ") + "/device/action" + 
                  " HTTP/1.1\r\n" +
                  "Host: " + "192.168.1.2" + "\r\n" + 
                  "User-Agent: Conesoft-Web-Button\r\n" +
                  "Conesoft-Web-Button-Id: " + WiFi.macAddress() + "::" + button + "\r\n" +
                  "Connection: close\r\n\r\n");
                  
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if(!!!client.available()) {
    error(3);
  }
  while(client.available()){
    client.read();
  }
  client.stop();
}

void handlePressed(int button) {
  if(isPressed(button)) {
    setLed(true);
    makeWebRequest(button);
  }
  while(isPressed(button)) {
    delay(10);
  }
}