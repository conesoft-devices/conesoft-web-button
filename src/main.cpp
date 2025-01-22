#include <DoubleResetDetector.h>
#include <WiFiSettings.h>
#include <LittleFS.h>
#include "helpers.h"

DoubleResetDetector drd(10, 0);

void setup() {
  Serial.begin(9600);
  Serial.println("booting up conesoft-web-button");
  pinMode(LED_BUILTIN, OUTPUT);
  LittleFS.begin();

  WiFiSettings.hostname = "csft-btn-";
  String host = WiFiSettings.string("host", "", "Conesoft Web Devices Server");
  int port = WiFiSettings.integer("port", 0, 65535, 0, "Conesoft Web Devices Server Port");

  if (drd.detectDoubleReset()) {
    WiFiSettings.portal();
  }

  WiFiSettings.connect();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  drd.loop();
}