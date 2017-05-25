#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

struct _apConfig;
typedef _apConfig APConfig;

APConfig* parseAPConfig(const char* configJSON);
bool startAP(APConfig &config);
