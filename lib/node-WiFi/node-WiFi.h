#ifndef __NODE_WIFI__
#define __NODE_WIFI__

#include "ESP8266WiFi.h"
#include <string>
#include <vector>
#include <node-Helper.h>
#include <node-Config.h>
#include <node-Query.h>
#include <node-Module.h>

typedef std::vector<std::string> listAP_Type;
listAP_Type* scanAP(bool inclHidden = false){
  WiFi.mode(WIFI_AP_STA);
  int _nets = WiFi.scanNetworks(false, inclHidden);
  listAP_Type* _apList = new listAP_Type();
  for (int i=0; i<_nets; i++)
    _apList->push_back(WiFi.SSID(i).c_str());
  return _apList;
};
bool validIP(std::string ip){
  IPAddress _ip;
  if (!ip.empty() && ip != "") return _ip.fromString(ip.c_str());
  else return false;
};
IPAddress stringToIP(std::string ip){
  IPAddress _ip;
  if (validIP(ip)) _ip.fromString(ip.c_str());
  return _ip;
};

class nodeAPConfig: public nodeConfig{
private:
  std::string _ssid;
  std::string _password;
  int _channel;
  bool _hidden;
  std::string _localIP;
  std::string _gateway;
  std::string _subnet;
protected:
  virtual bool parseString(std::string value){
    if (nodeConfig::parseString(value)){
      DynamicJsonBuffer _buffer(value.length());
      JsonObject& _obj = _buffer.parseObject(value.c_str());
      if (!this->setSSID((const char*)_obj["ssid"]))
        return doLog<bool>("AP-CONF: Missing SSID to set", false);
      if (_obj["password"].is<const char*>()) this->setPassword((const char*)_obj["password"]);
      if (!this->setChannel(_obj["channel"]))
        return doLog<bool>("AP-CONF: Invalid channel to set", false);
      if (_obj["hidden"].is<bool>()) this->setHidden(_obj["hidden"]);
      if (!this->setLocalIP((const char*)_obj["localIP"]))
        return doLog<bool>("AP-CONF: Invalid local IP to set", false);
      if (!this->setGateway((const char*)_obj["gateway"]))
        return doLog<bool>("AP-CONF: Invalid gateway IP to set", false);
      if (!this->setSubnet((const char*)_obj["subnet"]))
        return doLog<bool>("AP-CONF: Invalid subnet mask to set", false);
      return true;
    }
    else return doLog<bool>("AP-CONF: Unable to parse string", false);
  };
  virtual void defaultConfig(){
    std::string _cid = String(ESP.getChipId(), HEX).c_str();
    this->setSSID("node-AP"+_cid);
    this->setPassword("node-AP");
    this->setChannel(6);
    this->setHidden(false);
    this->setLocalIP("192.168.4.1");
    this->setGateway("192.168.4.1");
    this->setSubnet("255.255.255.0");
  };
public:
  static nodeAPConfig* getDefault(){
    nodeAPConfig* _conf = new nodeAPConfig();
    _conf->defaultConfig();
    return _conf;
  };
  nodeAPConfig(): nodeConfig("node-AP"){ nodeConfig::init(); };
  std::string getSSID(){ return this->_ssid; };
  bool setSSID(std::string value){
    if (!value.empty() && value != ""){
      this->_ssid = value;
      return true;
    }
    else return false;
  };
  std::string getPassword(){ return this->_password; };
  bool setPassword(std::string value){ this->_password = value; return true; };
  int getChannel(){ return this->_channel; };
  bool setChannel(int value){
    if (value > 0 && value <= 13){
      this->_channel = value;
      return true;
    }
    else return false;
  };
  bool getHidden(){ return this->_hidden; };
  bool setHidden(bool value){ this->_hidden = value; return true; };
  std::string getLocalIP(){ return this->_localIP; };
  bool setLocalIP(std::string value){
    if (validIP(value)){
      this->_localIP = value;
      return true;
    }
    else return false;
  };
  std::string getGateway(){ return this->_gateway; };
  bool setGateway(std::string value){
    if (validIP(value)){
      this->_gateway = value;
      return true;
    }
    else return false;
  };
  std::string getSubnet(){ return this->_subnet; };
  bool setSubnet(std::string value){
    if (validIP(value)){
      this->_subnet = value;
      return true;
    }
    else return false;
  };
  virtual queryResponse* execute(nodeQuery* query){
    queryResponse* _res = queryResponse::create(query);
    if (query != NULL){
      if (query->getModule() == "node-WiFi" && query->getSubModule() == "node-AP"){
        if (query->getCommand() == "set"){
          if (query->hasArguments()){
            if (!this->parseString(query->getArguments())) _res = _res->changeCode(-1)->changeMessage("Unable to set config");
            else{
              DynamicJsonBuffer _buffer(query->getArguments().length());
              JsonObject& _obj = _buffer.parseObject(query->getArguments().c_str());
              if (_obj["save"] == true){
                if (this->save()) _res = _res->addData("config", this->toString());
                else _res = _res->changeCode(-1)->changeMessage("Unable to save config");
              }
              else _res = _res->addData("config", this->toString());
            }
          }
          else _res = _res->changeCode(-1)->changeMessage("Missing config data");
        }
        else if (query->getCommand() == "get") _res = _res->addData("config", this->toString());
        else _res = _res->changeCode(-1)->changeMessage("Unsupported command");
      }
      else _res = _res->changeCode(-1)->changeMessage("Invalid query to execute");
    }
    else _res = _res->changeCode(-1)->changeMessage("Missing query to execute");
    return _res;
  };
  virtual std::string toString(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.parseObject(nodeConfig::toString().c_str());
    _obj["ssid"] = this->getSSID().c_str();
    _obj["password"] = this->getPassword().c_str();
    _obj["channel"] = this->getChannel();
    _obj["hidden"] = this->getHidden();
    _obj["localIP"] = this->getLocalIP().c_str();
    _obj["gateway"] = this->getGateway().c_str();
    _obj["subnet"] = this->getSubnet().c_str();
    return stringify(_obj);
  };
  bool isValid(){
    return !this->_ssid.empty() && this->_ssid != "" &&
      this->_channel > 0 && this->_channel <= 13 &&
      validIP(this->_localIP) &&
      validIP(this->_gateway) &&
      validIP(this->_subnet);
  };
};

class nodeSTAConfig: public nodeConfig{
private:
  std::string _ssid;
  std::string _password;
  bool _autoSwitch;
protected:
  virtual bool parseString(std::string value){
    if (nodeConfig::parseString(value)){
      DynamicJsonBuffer _buffer(value.length());
      JsonObject& _obj = _buffer.parseObject(value.c_str());
      if (!this->setSSID((const char*)_obj["ssid"])) return doLog<bool>("STA-CONF: Missing SSID to set", false);
      if (_obj["password"].is<const char*>()) this->setPassword((const char*)_obj["password"]);
      return true;
    }
    else return doLog<bool>("STA-CONF: unable to parse string", false);
  };
  virtual void defaultConfig(){
    this->setSSID("node-STA");
    this->setPassword("");
  };
public:
  nodeSTAConfig(): nodeConfig("node-STA"){ nodeConfig::init(); };
  std::string getSSID(){ return this->_ssid; };
  bool setSSID(std::string value){
    if (!value.empty() && value != ""){
      this->_ssid = value;
      return true;
    }
    else return false;
  };
  std::string getPassword(){ return this->_password; };
  bool setPassword(std::string value){ this->_password = value; return true; };
  bool getAutoSwitch(){ return this->_autoSwitch; };
  bool setAutoSwitch(bool value){ this->_autoSwitch = value; return true; };
  virtual queryResponse* execute(nodeQuery* query){
    queryResponse* _res = queryResponse::create(query);
    if (query == NULL){
      if (query->getModule() == "node-WiFi" && query->getSubModule() == "node-STA"){
        if (query->getCommand() == "set"){
          if (!this->parseString(query->getArguments())) _res = _res->changeCode(-1)->changeMessage("Unable to set config");
          else{
            DynamicJsonBuffer _buffer(query->getArguments().length());
            JsonObject& _obj = _buffer.parseObject(query->getArguments().c_str());
            if (_obj["save"] == true){
              if (this->save()) _res = _res->addData("config", this->toString());
              else _res = _res->changeCode(-1)->changeMessage("Unable to save config");
            }
            else _res = _res->addData("config", this->toString());
          }
        }
        else if (query->getCommand() == "get") _res = _res->addData("config", this->toString());
        else _res = _res->changeCode(-1)->changeMessage("Unsupported command");
      }
      else _res = _res->changeCode(-1)->changeMessage("Invalid query to execute");
    }
    else _res = _res->changeCode(-1)->changeMessage("Missing query to execute");
    return _res;
  };
  virtual std::string toString(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.parseObject(nodeConfig::toString().c_str());
    _obj["ssid"] = this->getSSID().c_str();
    _obj["password"] = this->getPassword().c_str();
    return stringify(_obj);
  };
  bool isValid(){ return !this->_ssid.empty() && this->_ssid != ""; };
};

class nodeWifi: public nodeModule{
private:
  bool _apEnabled;
  bool startAP(nodeAPConfig* config){
    if (!this->stopAP()) return doLog<bool>("node-WiFi: Unable disable AP", false);
    if (config->isValid()){
      if (!WiFi.softAPConfig(stringToIP(config->getLocalIP()), stringToIP(config->getGateway()), stringToIP(config->getSubnet())))
        return doLog<bool>("node-WiFi: Unable setup AP IP", false);
      if (!WiFi.softAP(config->getSSID().c_str(), config->getPassword().c_str(), config->getChannel(), config->getHidden() ? 1 : 0))
        return doLog<bool>("node-WiFi: Unable start AP", false);
      return doLog<bool>("node-WiFi: AP started, SSID: "+config->getSSID(), true);
    }
    else return doLog<bool>("node-WiFi: Invalid AP config", false);
  };
  bool stopAP(){
    if (_apEnabled)
      _apEnabled = !WiFi.softAPdisconnect();
    doLog(_apEnabled ? "node-WiFi: AP still online" : "node-WiFi: AP disabled");
    return _apEnabled;
  };
  bool connectSTA(nodeSTAConfig* config){
    if (WiFi.SSID() == config->getSSID().c_str())
      return doLog<bool>("node-WiFi: STA already connected", true);
    if (!this->disconnectSTA()) return doLog<bool>("node-WiFi: Unable disconnect STA", false);
    if (config->isValid()){
      WiFi.begin(config->getSSID().c_str(), config->getPassword().c_str());
      while (WiFi.status() == WL_IDLE_STATUS) delay(100);
      switch (WiFi.status()){
        case WL_CONNECTED:
          if (config->getAutoSwitch()) WiFi.softAPdisconnect();
          return doLog<bool>("node-WiFi: STA connected, SSID: "+config->getSSID(), true);
        case WL_CONNECT_FAILED: return doLog<bool>("node-WiFi: STA failed, invalid password", false);
        case WL_NO_SSID_AVAIL: return doLog<bool>("node-WiFi: STA failed, no SSID "+config->getSSID(), false);
        default: return doLog<bool>("node-WiFi: STA failed, disconnected", false);
      };
    }
    else return doLog<bool>("node-WiFi: Invalid STA config", false);
  };
  bool disconnectSTA(){
    if (this->isConnected())
      WiFi.disconnect();
    delay(100);
    doLog(this->isConnected() ? "node-WiFi: STA still connected" : "node-WiFi: STA disconnected");
    return !this->isConnected();
  };
public:
  nodeWifi(): nodeModule(){};
  virtual bool setup(){
    WiFi.mode(WIFI_AP_STA);
    nodeAPConfig* _apConfig = new nodeAPConfig();
    if (_apConfig->load()){
      if (!this->startAP(_apConfig)){
        _apConfig = nodeAPConfig::getDefault();
        this->startAP(_apConfig);
      }
    }
    nodeSTAConfig* _staConfig = new nodeSTAConfig();
    if (_staConfig->load())
      this->connectSTA(_staConfig);
    return this->isAPEnabled() || this->isConnected();
  };
  virtual bool loop(){};
  virtual std::string execute(nodeQuery* query){};
  inline bool isAPEnabled(){ return this->_apEnabled; };
  bool isConnected(){
    return WiFi.status() == WL_CONNECTED || WiFi.status() == WL_IDLE_STATUS;
  };
  uint8_t getStatus(){ return WiFi.status(); };
};

#endif
