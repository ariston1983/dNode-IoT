#include <node-WiFi.h>

typedef struct _apConfig{
  const char* ssid;
  const char* password;
  int channel;
  bool hidden;
} APConfig;

APConfig* parseAPConfig(const char* json){
  DynamicJsonBuffer _buffer(512);
  JsonVariant _parsed = _buffer.parse(json);
  if (_parsed.is<JsonObject>()){
    JsonObject& _jConfig = _parsed;
    APConfig* _config = new APConfig();
    _config->ssid = _jConfig["ssid"];
    _config->password = _jConfig["password"];
    _config->channel = _jConfig["channel"];
    _config->hidden = _jConfig["hidden"];
    return _config;
  }
  else return NULL;
}
bool startAP(APConfig &config){
  return false;
}
