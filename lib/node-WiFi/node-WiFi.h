#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <node-Config.h>

int WiFi_scanAP(String *&ssid);
IPAddress stringToIP(const char* ip);

class APControl{
private:
  nConfig* _config;
public:
  APControl();
  APControl(const char* ssid, const char* password = NULL, int channel = 6, bool hidden = false);
  bool valid();
  bool startAP();
  bool stopAP();
  template <typename TValue>
  void setConfig(String key, TValue value){
    if (key == "localIP" || key == "gateway" || key == "subnet"){
      IPAddress _ip;
      if (!_ip.fromString(value)) return;
    }
    return this->_config->set<TValue>(key.c_str(), value);
  };
  template <typename TReturn>
  TReturn getConfig(String key){
    return this->_config->get<TReturn>(key.c_str());
  };
  bool saveConfig();
};

class STAControl{
private:
  const char* ssid;
  const char* password;
public:
  STAControl(const char* ssid, const char* password = NULL);
  bool valid();
  bool parseConfig(const char* json);
  bool connect();
  bool disconnect();
};
