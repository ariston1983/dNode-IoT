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
bool APConfig::isValid(){
  return
    this->_ssid != "" &&
    this->_channel > 0 && this->_channel <= 13 &&
    this->_localIP != "" &&
    this->_gateway != "" &&
    this->_subnet != "";
};

nodeAP::nodeAP(nodeAPConfig config){};
bool nodeAP::startAP(){
  if (this->_config->isValid()){
    IPAddress _localIP = this->_config->localIP();
    IPAddress _gateway = this->_config->gateway();
    IPAddress _subnet = this->_config->subnet();
    if (!WiFi.softAPConfig(_localIP, _gateway, _subnet)) return false;
    const char* _password = NULL;
    if (!this->_config->isEqual<const char*>("password", "")) _password = this->_config->get<const char*>("password");
    int _hidden = 0;
    if (this->_config->isEqual<bool>("hidden", true)) _hidden = 1;
    return WiFi.softAP(
      this->_config->get<const char*>("ssid"),
      _password,
      this->_config->get<int>("channel"),
      _hidden);
  }
  else return false;
};
bool nodeAP::stopAP(){
  return WiFi.softAPdisconnect(true);
};

nodeSTA::nodeSTA(nodeSTAConfig config){
  this->_config = config;
};
bool nodeSTA::connect(){};
bool nodeSTA::disconnect(){};
