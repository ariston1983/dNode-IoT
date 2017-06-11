#include <ESP8266WiFi.h>
#include <node-Config.h>

int WiFi_scanAP(String *&ssid);
IPAddress stringToIP(const char* ip);

class STAConfig : public nodeConfig{
public:
  static STAConfig* defaultConfig();
  inline STAConfig(): nodeConfig("node-STA"){ };
  bool isValid();
  String ssid(String value = "");
  String password(String value = "");
  virtual String toString() override;
};
class APConfig : public nodeConfig{
public:
  static APConfig* defaultConfig();
  inline APConfig(): nodeConfig("node-AP"){ };
  bool isValid();
  String ssid(String value = "");
  IPAddress localIP(String value = "192.168.4.1");
  IPAddress gateway(String value = "192.168.4.1");
  IPAddress subnet(String value = "255.255.255.0");
  virtual String toString() override;
};
typedef STAConfig* nodeSTAConfig;
typedef APConfig* nodeAPConfig;

class nodeAP{
private:
  nodeAPConfig _config;
public:
  nodeAP(nodeAPConfig config = NULL);
  nodeAPConfig config();
  bool startAP();
  bool stopAP();
  bool reset();
};

class nodeSTA{
private:
  nodeSTAConfig _config;
public:
  nodeSTA(nodeSTAConfig config = NULL);
  nodeSTAConfig config();
  bool connect();
  bool disconnect();
};
