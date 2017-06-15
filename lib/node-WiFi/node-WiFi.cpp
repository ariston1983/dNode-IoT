#include <node-WiFi.h>

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

bool STAConfig::parseString(String toParse){
  if (nodeConfig::parseString(toParse)){
    DynamicJsonBuffer _buffer(toParse.length());
    JsonVariant _var = _buffer.parseObject(toParse.c_str());
    JsonObject& _obj = _var.as<JsonObject>();
    this->_ssid = _obj["ssid"];
    this->_password = _obj["password"];
    return true;
  }
  else return false;
};
void STAConfig::defaultConfig(){
  this->_ssid = "";
  this->_password = "";
};

bool APConfig::parseString(String toParse){
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
    return true;
  }
  else return false;
};
void APConfig::defaultConfig(){
  this->_ssid = "node-AP";
  this->_password = "1234567";
  this->_channel = 6;
  this->_hidden = false;
  this->_localIP = "192.168.4.1";
  this->_gateway = "192.168.4.1";
  this->_subnet = "255.255.255.0";
};
String APConfig::query(nodeQuery* query){
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
  Serial.println("Processing query complete");
  String _resStr = stringify(_res);
  Serial.println("Query response");
  Serial.println(_resStr);
  return _resStr;
};
bool APConfig::isValid(){
  return
    this->_ssid != NULL && this->_ssid != "" &&
    this->_channel > 0 && this->_channel <= 13 &&
    this->_localIP != NULL && this->_localIP != "" &&
    this->_gateway != NULL && this->_gateway != "" &&
    this->_subnet != NULL && this->_subnet != "";
};
IPAddress APConfig::getLocalIPA(){ return stringToIP(this->_localIP); };
IPAddress APConfig::getGatewayIPA(){ return stringToIP(this->_gateway); };
IPAddress APConfig::getSubnetIPA(){ return stringToIP(this->_subnet); };
String APConfig::toString(){
  String _baseString = nodeConfig::toString();
  DynamicJsonBuffer _buffer(_baseString.length());
  JsonObject& _obj = _buffer.parseObject(_baseString.c_str());
  _obj["ssid"] = this->_ssid;
  _obj["password"] = this->_password;
  _obj["channel"] = this->_channel;
  _obj["hidden"] = this->_hidden;
  _obj["localIP"] = this->_localIP;
  _obj["gateway"] = this->_gateway;
  _obj["subnet"] = this->_subnet;
  return stringify(_obj);
};

bool nodeAP::loadConfig(){
  this->_config = new APConfig();
  return this->_config->load();
};
nodeConfig& nodeAP::getConfig(){
  return *this->_config;
};
const char* query(nodeQuery* query){

};
bool nodeAP::start(){
  if (this->isRunning()){
    Serial.println("AP already running");
    return false;
  };
  if (!this->_config->isValid()){
    Serial.println("Invalid AP configuration");
    return false;
  };
  if (!WiFi.softAPConfig(this->_config->getLocalIPA(), this->_config->getGatewayIPA(), this->_config->getSubnetIPA())){
    Serial.println("Unable setup AP IPs");
    return false;
  }
  int _hidden = (this->_config->getHidden() ? 1 : 0);
  if (!WiFi.softAP(this->_config->getSSID(), this->_config->getPassword(), this->_config->getChannel(), _hidden)){
    Serial.println("Unable start AP");
    return false;
  }
  Serial.println("AP established");
  return true;
};
bool nodeAP::pause(){
  Serial.println("Pausing AP is not supported");
  return false;
};
bool nodeAP::stop(){
  bool _res = WiFi.softAPdisconnect(false);
  Serial.println(_res ? "AP disconnected" : "Failed to disconnect AP");
  return _res;
};

bool nodeSTA::loadConfig(){
  this->_config = new STAConfig();
  return this->_config->load();
};
nodeConfig& nodeSTA::getConfig(){
  return *this->_config;
};
bool nodeSTA::start(){
  return false;
};
bool nodeSTA::pause(){
  return false;
};
bool nodeSTA::stop(){
  return false;
};
