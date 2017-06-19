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
IPAddress stringToIP(std::string ip){
  IPAddress _ip;
  if (!ip.empty() && ip != "") _ip.fromString(ip.c_str());
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
      this->_ssid = (const char*)_obj["ssid"];
      this->_password = (const char*)_obj["password"];
      this->_channel = _obj["channel"];
      this->_hidden = _obj["hidden"];
      this->_localIP = (const char*)_obj["localIP"];
      this->_gateway = (const char*)_obj["gateway"];
      this->_subnet = (const char*)_obj["subnet"];
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
    queryResponse* _res = queryResponse::create(query);
    if (query->getModule() == "node-AP" && query->getSubModule() == "config"){
      if (query->getCommand() == "set" && query->hasArguments()){
        DynamicJsonBuffer _argBuff(query->getArguments().length());
        JsonObject& _args = _argBuff.createObject();
        bool _valid = (_args["ssid"].as<const char*>() != "") &&
          (_args["channel"].as<int>() > 0 && _args["channel"].as<int>() <= 13) &&
          (_args["hidden"].is<bool>()) &&
          (_args["localIP"].as<const char*>() != "") &&
          (_args["gateway"].as<const char*>() != "") &&
          (_args["subnet"].as<const char*>() != "");
        if (_valid){
          this->_ssid = (const char*)_args["ssid"];
          this->_password = (const char*)_args["password"];
          this->_channel = _args["channel"];
          this->_hidden = _args["hidden"];
          this->_localIP = (const char*)_args["localIP"];
          this->_gateway = (const char*)_args["gateway"];
          this->_subnet = (const char*)_args["subnet"];
          if (_args["save"] == true && !this->save())
            _res = _res->
              changeCode(-1)->
              changeMessage("Fail save config")->
              addData("config", this->toString());
          else
            _res = _res->addData("config", this->toString());
        }
        else
        _res = _res->
          changeCode(-1)->
          changeMessage("Invalid config");
      }
      else if (query->getCommand() == "get")
        _res = _res->addData("config", this->toString());
      else
        _res = _res->
          changeCode(-1)->
          changeMessage("Not supported command")->
          addData("command", query->getCommand());
    }
    else
      _res = _res->
        changeCode(-1)->
        changeMessage("Invalid query");
    std::string _resStr = _res->toString();
    return doLog<std::string>("APConfig: Query response: "+_resStr, _resStr);
  };
  bool isValid(){
    return
      !this->_ssid.empty() && this->_ssid != "" &&
      this->_channel > 0 && this->_channel <= 13 &&
      !this->_localIP.empty() && this->_localIP != "" &&
      !this->_gateway.empty() && this->_gateway != "" &&
      !this->_subnet.empty() && this->_subnet != "";
  };
  inline std::string getSSID(){ return this->_ssid; };
  inline std::string getPassword(){ return this->_password; };
  inline int getChannel(){ return this->_channel; };
  inline bool getHidden(){ return this->_hidden; }
  inline IPAddress getLocalIPA(){ return stringToIP(this->_localIP); };
  inline IPAddress getGatewayIPA(){ return stringToIP(this->_gateway); };
  inline IPAddress getSubnetIPA(){ return stringToIP(this->_subnet); };
  virtual std::string toString(){
    std::string _baseString = nodeConfig::toString();
    DynamicJsonBuffer _buffer(_baseString.length());
    JsonObject& _obj = _buffer.parseObject(_baseString.c_str());
    _obj["ssid"] = this->_ssid.c_str();
    _obj["password"] = this->_password.c_str();
    _obj["channel"] = this->_channel;
    _obj["hidden"] = this->_hidden;
    _obj["localIP"] = this->_localIP.c_str();
    _obj["gateway"] = this->_gateway.c_str();
    _obj["subnet"] = this->_subnet.c_str();
    return stringify(_obj);
  };
};
typedef APConfig* nodeAPConfig;
class nodeAP : public nodeModule{
private:
  nodeAPConfig _config;
public:
  inline nodeAP() : nodeModule(){ };
  inline virtual nodeConfig* getConfig(){ return this->_config; };
  virtual bool setup(){
    this->_config = new APConfig();
    if (this->_config->load()){
      if (this->_config->isValid()){
        WiFi.softAPdisconnect();
        WiFi.softAPConfig(
          this->_config->getLocalIPA(),
          this->_config->getGatewayIPA(),
          this->_config->getSubnetIPA());
        return WiFi.softAP(
          this->_config->getSSID().c_str(),
          this->_config->getPassword().c_str(),
          this->_config->getChannel(),
          this->_config->getHidden() ? 1 : 0);
      }
      else return doLog<bool>("Invalid saved AP config", false);
    }
    else return doLog<bool>("Unable load AP config", false);
  };
  virtual bool loop(){ return false; };
  virtual std::string execute(nodeQuery* query){
    if (query != NULL){
      if (!query->getSubModule().empty() && query->getSubModule() == "config")
        return this->_config->execute(query);
      else{
        DynamicJsonBuffer _resBuff(512);
        JsonObject& _obj = _resBuff.createObject();
      }
    }
    else return doLog<std::string>("nodeAP: query not found", "");
    return "";
  };
};

class STAConfig : public nodeConfig{
private:
  std::string _ssid;
  std::string _password;
protected:
  virtual bool parseString(std::string toParse){
    if (nodeConfig::parseString(toParse)){
      DynamicJsonBuffer _buffer(toParse.length());
      JsonVariant _var = _buffer.parseObject(toParse.c_str());
      JsonObject& _obj = _var.as<JsonObject>();
      this->_ssid = (const char*)_obj["ssid"];
      this->_password = (const char*)_obj["password"];
      return true;
    }
    else return false;
  };
  virtual void defaultConfig(){
    this->_ssid = "";
    this->_password = "";
  };
public:
  inline STAConfig(): nodeConfig("node-STA"){ nodeConfig::init(); };
  inline bool isValid(){ return !this->_ssid.empty() && this->_ssid != ""; };
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
  virtual bool setup(){ return false; };
  virtual std::string execute(nodeQuery *query){ return ""; };
  virtual bool loop(){ return false; };
};

#endif
