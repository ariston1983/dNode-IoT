#ifndef __NODE_WIFI__
#define __NODE_WIFI__

#include "ESP8266WiFi.h"
#include <string>
#include <node-Config.h>
#include <node-Query.h>
#include <node-Module.h>

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
  inline STAConfig(): nodeConfig("node-STA"){ nodeConfig::init(); };
  inline bool isValid(){ return this->_ssid != NULL && this->_ssid != ""; };
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
  inline APConfig(): nodeConfig("node-AP"){ nodeConfig::init(); };
  virtual String query(nodeQuery* query) override;
  bool isValid();
  inline const char* getSSID(){ return this->_ssid; };
  inline const char* getPassword(){ return this->_password; };
  inline int getChannel(){ return this->_channel; };
  inline bool getHidden(){ return this->_hidden; }
  IPAddress getLocalIPA();
  IPAddress getGatewayIPA();
  IPAddress getSubnetIPA();
  virtual String toString() override;
};
typedef STAConfig* nodeSTAConfig;
typedef APConfig* nodeAPConfig;

class nodeAP : public nodeModule{
private:
  nodeAPConfig _config;
protected:
  virtual bool loadConfig() override;
public:
  inline nodeAP() : nodeModule(){ };
  virtual nodeConfig& getConfig();
  virtual const char* query(nodeQuery* query) override;
  virtual bool start() override;
  virtual bool pause() override;
  virtual bool stop() override;
};

class nodeSTA : public nodeModule{
private:
  nodeSTAConfig _config;
protected:
  virtual bool loadConfig() override;
public:
  inline nodeSTA(): nodeModule(){ };
  virtual nodeConfig& getConfig();
  virtual bool start() override;
  virtual bool pause() override;
  virtual bool stop() override;
};

#endif
