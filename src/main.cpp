#include "csft-lib.h"

const int BUTTON1 = 4;
const int BUTTON2 = 5;

String webRequest;

void setup()
{
  csft_setup("conesoft-web-button", []() -> void
             { webRequest = WiFiSettings.string("webrequest", "", "Conesoft Web Devices Web Request Target"); });

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
}

void handlePressed(int button);

void loop()
{
  csft_loop();

  handlePressed(BUTTON1);
  handlePressed(BUTTON2);
  digitalWrite(LED_BUILTIN, HIGH);
}

void handlePressed(int button)
{
  if (digitalRead(button) == LOW)
  {
    digitalWrite(LED_BUILTIN, LOW);
    csft_web_request(webRequest, "Conesoft-Web-Button", WiFi.macAddress() + "::" + button);
  }
  while (digitalRead(button) == LOW)
  {
    delay(10);
  }
}