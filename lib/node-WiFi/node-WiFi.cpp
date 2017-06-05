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

void STAConfig::reset(){
  nodeConfig::reset();
  this->set<const char*>("ssid", "unknown");
  this->set<const char*>("password", NULL);
};
bool STAConfig::isValid(){
  return !this->isEqual<String>("ssid", "");
};

void APConfig::reset(){
  nodeConfig::reset();
  this->set<const char*>("ssid", "node-AP");
  this->set<const char*>("password", "node-AP");
  this->set<int>("channel", 6);
  this->set<bool>("hidden", false);
  this->set<const char*>("localIP", "192.168.4.1");
  this->set<const char*>("gateway", "192.168.4.1");
  this->set<const char*>("subnet", "255.255.255.0");
};
bool APConfig::isValid(){
  return
    !this->isEqual("ssid", "") &&
    (this->get<int>("channel") > 0 && this->get<int>("channel") <= 13) &&
    !this->isEqual("localIP", "") &&
    !this->isEqual("gateway", "") &&
    !this->isEqual("subnet", "");
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
