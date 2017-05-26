#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

int WiFi_scanAP(String *&ssid);

class APControl{
private:
  IPAddress localIP;
  IPAddress gateway;
  IPAddress subnet;
  const char* ssid;
  const char* password;
  int channel;
  bool hidden;
public:
  APControl();
  APControl(const char* ssid, const char* password = NULL, int channel = 6, bool hidden = false);
  APControl(const char* localIP, const char* gateway, const char* subnet, const char* password = NULL, int channel = 6, bool hidden = false);
  bool valid();
  bool parseConfig(const char* json);
  bool startAP();
  bool stopAP();
  const char* getSSID();
  const char* getPassword();
  int getChannel();
  bool getHidden();
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
