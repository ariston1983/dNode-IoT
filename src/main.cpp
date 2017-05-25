#include "Arduino.h"
#include "FS.h"
#include <node-WiFi.h>

WiFiClient wClient;

void setup();
void loop();
void setup_wifi();

void setup(){
  parseAPConfig("{\"ssid\": \"dodol-AP\", \"password\": \"dodolPWD\", \"channel\": 6, \"hidden\": false}");
}
void loop(){}
void setup_wifi(){}
