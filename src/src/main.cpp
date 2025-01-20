#include <ESP8266WiFi.h>

#include "config.h"
#include "helpers.h"

/* main */
void initWifi();
void makeWebRequest(int button);
void handlePressed(int button);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  setLed(true);
  initWifi();
  setLed(false);
}

void loop() {
  while(true) {
    handlePressed(BUTTON1);
    handlePressed(BUTTON2);
    setLed(false);
    delay(10);
  }
}

void initWifi() {
  WiFi.persistent(false);
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
  }
  if(WiFi.status() != WL_CONNECTED) {
    error(1);
  }
}

void makeWebRequest(int button) {
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, port) && (retries-- > 0)) {
  }
  if(!!!client.connected()) {
    error(2);
  }
  client.print(String("GET ") + resource + 
                  " HTTP/1.1\r\n" +
                  "Host: " + server + "\r\n" + 
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