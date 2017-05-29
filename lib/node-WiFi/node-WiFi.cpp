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

APControl::APControl(void){
  this->_config = loadConfig("wifi-AP");
   if (!this->_config->has("localIP"))
     this->_config->set<const char*>("localIP", "192.168.4.2");
  // if (!this->_config->has("gateway"))
  //   this->_config->set<const char*>("gateway", "192.168.4.1");
  // if (!this->_config->has("subnet"))
  //   this->_config->set<const char*>("subnet", "255.255.255.0");
  // if (!this->_config->has("ssid"))
  //   this->_config->set<const char*>("ssid", "node-AP");
  // if (!this->_config->has("password"))
  //   this->_config->set<const char*>("password", "node-AP");
  // if (!this->_config->has("channel"))
  //   this->_config->set<int>("channel", 6);
  // if (!this->_config->has("hidden"))
  //   this->_config->set<bool>("hidden", false);
  Serial.println("AP: default initialization complete");
};
APControl::APControl(const char* ssid, const char* password, int channel, bool hidden)
  : APControl(){
  if (ssid == NULL || ssid == "") this->_config->set("ssid", ssid);
  this->_config->set("password", password);
  if (channel >= 1 && channel <= 13) this->_config->set("channel", channel);
  this->_config->set("hidden", hidden);
  Serial.println("AP: custom initialization complete");
};
bool APControl::valid(){
  IPAddress _ip;
  if (!this->_config->isEqual<const char*>("localIP", "")) return false;
  if (!_ip.fromString(this->_config->get<const char*>("localIP"))) return false;
  if (!this->_config->isEqual<const char*>("gateway", "")) return false;
  if (!_ip.fromString(this->_config->get<const char*>("gateway"))) return false;
  if (!this->_config->isEqual<const char*>("subnet", "")) return false;
  if (!_ip.fromString(this->_config->get<const char*>("subnet"))) return false;
  if (!this->_config->isEqual<const char*>("ssid", "")) return false;
  if (this->_config->get<int>("channel") < 1 || this->_config->get<int>("channel") > 13) return false;
  return true;
};
bool APControl::startAP(){
  if (this->valid()){
    IPAddress _localIP; _localIP.fromString(this->_config->get<const char*>("localIP"));
    IPAddress _gateway; _gateway.fromString(this->_config->get<const char*>("gateway"));
    IPAddress _subnet; _subnet.fromString(this->_config->get<const char*>("subnet"));
    if (!WiFi.softAPConfig(_localIP, _gateway, _subnet)) return false;
    const char* password = NULL;
    if (!this->_config->isEqual<const char*>("password", "")) password = this->_config->get<const char*>("password");
    return WiFi.softAP(
      this->_config->get<const char*>("ssid"),
      password,
      this->_config->get<int>("channel"),
      this->_config->get<bool>("hidden") ? 1 : 0);
  }
  else return false;
};
bool APControl::stopAP(){
  return WiFi.softAPdisconnect(true);
};
bool APControl::saveConfig(){
  Serial.println("AP: saving config");
  return this->_config->save();
};

STAControl::STAControl(const char* ssid, const char* password){
  this->ssid = ssid;
  this->password = password;
};
bool STAControl::valid(){

};
bool STAControl::parseConfig(const char *json){};
bool STAControl::connect(){};
bool STAControl::disconnect(){};
