#include <ESP8266WiFi.h>
#include <node-Config.h>

int WiFi_scanAP(String *&ssid);
IPAddress stringToIP(const char* ip);

class STAConfig : public nodeConfig{
protected:
  void reset();
public:
  inline STAConfig(): nodeConfig("node-STA"){};
  bool isValid();
};
class APConfig : public nodeConfig{
protected:
  void reset();
public:
  inline APConfig(): nodeConfig("node-AP"){};
  bool isValid();
  IPAddress localIP(String def = "192.168.4.1");
  IPAddress gateway(String def = "192.168.4.1");
  IPAddress subnet(String def = "255.255.255.0");
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
