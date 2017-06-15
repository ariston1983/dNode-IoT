#include <ESP8266WiFi.h>
#include <node-Config.h>

int WiFi_scanAP(String *&ssid);
IPAddress stringToIP(const char* ip);

class STAConfig : public nodeConfig{
private:
  const char* _ssid;
  const char* _password;
protected:
  virtual bool parseString(String toParse) override;
  virtual void defaultConfig() override;
public:
  inline STAConfig(): nodeConfig("node-STA"){ };
  inline bool isValid(){ return this->_ssid != ""; };
  inline void setSSID(const char* ssid){ if (ssid != "") this->_ssid = ssid; };
  inline const char* getSSID(){ return this->_ssid; };
  inline void setPassword(const char* password){ this->_password = password; };
  inline const char* getPassword(){ return this->_password; };
};
class APConfig : public nodeConfig{
private:
  const char* _ssid;
  const char* _password;
  int _channel;
  bool _hidden;
  const char* _localIP;
  const char* _gateway;
  const char* _subnet;
protected:
  virtual bool parseString(String toParse) override;
  virtual void defaultConfig() override;
public:
  inline APConfig(): nodeConfig("node-AP"){ };
  bool isValid();
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
