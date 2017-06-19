#ifndef __NODE_WIFI__
#define __NODE_WIFI__

#include "ESP8266WiFi.h"
#include <string>
#include <vector>
#include <node-Helper.h>
#include <node-Config.h>
#include <node-Query.h>
#include <node-Module.h>

typedef std::vector<std::string> APList_Type;
int WiFi_scanAP(String *&ssid){
  WiFi.mode(WIFI_AP_STA);
  int _nets = WiFi.scanNetworks(false, true);
  ssid = new String[_nets];
  for (int i=0; i<_nets; i++)
    ssid[i] = WiFi.SSID(i);
  return _nets;
};
IPAddress stringToIP(const char* ip){
  IPAddress _ip;
  if (ip != NULL && ip != "") _ip.fromString(ip);
  return _ip;
};

class APConfig : public nodeConfig{
private:
  std::string _ssid;
  std::string _password;
  int _channel;
  bool _hidden;
  std::string _localIP;
  std::string _gateway;
  std::string _subnet;
protected:
  virtual bool parseString(std::string toParse){
    if (nodeConfig::parseString(toParse)){
      DynamicJsonBuffer _buffer(toParse.length());
      JsonVariant _var = _buffer.parseObject(toParse.c_str());
      JsonObject& _obj = _var.as<JsonObject>();
      this->_ssid = _obj["ssid"];
      this->_password = _obj["password"];
      this->_channel = _obj["channel"];
      this->_hidden = _obj["hidden"];
      this->_localIP = _obj["localIP"];
      this->_gateway = _obj["gateway"];
      this->_subnet = _obj["subnet"];
      return doLog<bool>("APConfig: Parse string complete", true);
    }
    else return doLog<bool>("APConfig: Parse config failed", false);
  };
  virtual void defaultConfig(){
    this->_ssid = "node-AP";
    this->_password = "1234567";
    this->_channel = 6;
    this->_hidden = false;
    this->_localIP = "192.168.4.1";
    this->_gateway = "192.168.4.1";
    this->_subnet = "255.255.255.0";
  };
public:
  inline APConfig(): nodeConfig("node-AP"){ nodeConfig::init(); };
  virtual std::string execute(nodeQuery* query){
    const char* _reqQuery = query->toString().c_str();
    Serial.println("Processing query");
    Serial.println(_reqQuery);
    DynamicJsonBuffer _resBuff(512);
    JsonObject& _res = _resBuff.createObject();
    _res["module"] = "node-AP";
    if (query->getModule() == "node-AP"){
      if (query->getSubModule() == "config"){
        if (query->getCommand() == "set"){
          if (query->hasArguments()){
            DynamicJsonBuffer _argsBuff(sizeof(query->getArguments()));
            JsonObject& _args = _argsBuff.parseObject(query->getArguments());
            this->_ssid = _args["ssid"];
            this->_password = _args["password"];
            this->_channel = _args["channel"];
            this->_hidden = _args["hidden"];
            this->_localIP = _args["localIP"];
            this->_gateway = _args["gateway"];
            this->_subnet = _args["subnet"];
            bool _save = _args["save"];
            if (_save){
              if (this->isValid()){
                if (this->save()){
                  _res["code"] = 0;
                  _res["message"] = "OK";
                  _res["config"] = this->toString();
                }
                else{
                  _res["code"] = -1;
                  _res["message"] = "Unable to save new config";
                }
              }
              else{
                _res["code"] = -1;
                _res["message"] = "Invalid config, cannot save";
              }
            }
            else{
              _res["code"] = 0;
              _res["message"] = "OK";
              _res["config"] = this->toString();
            };
          }
          else{
            _res["code"] = -1;
            _res["message"] = "Missing config values";
          }
        }
        else if (query->getCommand() == "get"){
          _res["code"] = 0;
          _res["message"] = "OK";
          _res["config"] = this->toString().c_str();
        }
        else{
          _res["code"] = -1;
          _res["message"] = "Unsupported command";
        }
      }
      else{
        _res["code"] = -1;
        _res["message"] = "Invalid sub module, should set [config]";
      };
    }
    else{
      _res["code"] = -1;
      _res["message"] = "Invalid module";
    };
    std::string _resStr = stringify(_res);
    return doLog<std::string>("APConfig: Query response: "+_resStr, _resStr);
  };
  bool isValid();
  inline std::string getSSID(){ return this->_ssid; };
  inline std::string getPassword(){ return this->_password; };
  inline int getChannel(){ return this->_channel; };
  inline bool getHidden(){ return this->_hidden; }
  IPAddress getLocalIPA();
  IPAddress getGatewayIPA();
  IPAddress getSubnetIPA();
  virtual std::string toString() override;
};
typedef APConfig* nodeAPConfig;
class nodeAP : public nodeModule{
private:
  nodeAPConfig _config;
public:
  inline nodeAP() : nodeModule(){ };
  inline virtual nodeConfig* getConfig(){ return this->_config; };
  virtual bool init() override;
  virtual std::string execute(nodeQuery *query) override;
  virtual bool loop() override;
};

class STAConfig : public nodeConfig{
private:
  std::string _ssid;
  std::string _password;
protected:
  virtual bool parseString(std::string toParse) override;
  virtual void defaultConfig() override;
public:
  inline STAConfig(): nodeConfig("node-STA"){ nodeConfig::init(); };
  inline bool isValid(){ return this->_ssid != NULL && this->_ssid != ""; };
  inline void setSSID(std::string ssid){ if (ssid != "") this->_ssid = ssid; };
  inline std::string getSSID(){ return this->_ssid; };
  inline void setPassword(std::string password){ this->_password = password; };
  inline std::string getPassword(){ return this->_password; };
};
typedef STAConfig* nodeSTAConfig;
class nodeSTA : public nodeModule{
private:
  nodeSTAConfig _config;
public:
  inline nodeSTA(): nodeModule(){ };
  inline virtual nodeConfig* getConfig(){ return this->_config; };
  virtual std::string execute(nodeQuery *query);
  virtual bool loop();
};

#endif
