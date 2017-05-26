#include <node-WiFi.h>

APControl::APControl(void){
  this->localIP = IPAddress(192, 168, 4, 2);
  this->gateway = IPAddress(192, 168, 4, 1);
  this->subnet = IPAddress(255, 255, 255, 0);
  this->ssid = "node-AP";
  this->password = "node-AP";
  this->channel = 6;
  this->hidden = false;
};
APControl::APControl(const char* ssid, const char* password, int channel, bool hidden)
  : APControl(){
  if (ssid == NULL || ssid == "") ssid = "node-AP";
  this->ssid = ssid;
  this->password = password;
  if (channel >= 1 && channel <= 13) this->channel = channel;
  this->hidden = hidden;
};
APControl::APControl(const char* localIP, const char* gateway, const char* subnet, const char* password, int channel, bool hidden)
  : APControl(ssid, password, channel, hidden){
  IPAddress _ip;
  if (localIP != NULL && localIP != "" && _ip.fromString(localIP)) this->localIP.fromString(localIP);
  if (gateway != NULL && gateway != "" && _ip.fromString(gateway)) this->gateway.fromString(gateway);
  if (subnet != NULL && subnet != "" && _ip.fromString(subnet)) this->localIP.fromString(subnet);
}
bool APControl::valid(){
  if (&this->localIP == NULL) return false;
  if (&this->gateway == NULL) return false;
  if (&this->subnet == NULL) return false;
  if (this->ssid == NULL || this->ssid == "") return false;
  if (this->password == "") this->password = NULL;
  if (this->channel < 1 || this-> channel > 13) return false;
  return true;
};
bool APControl::parseConfig(const char* json){
  DynamicJsonBuffer _buffer(512);
  JsonVariant _parsed = _buffer.parse(json);
  if (_parsed.is<JsonObject>()){
    JsonObject& _config = _parsed;
    const char* _ip;
    IPAddress _valIP;
    if (_config["localIP"] != NULL && _config["localIP"] != ""){
      _ip = _config["localIP"];
      if (_valIP.fromString(_ip)) this->localIP.fromString(_ip);
      else return false;
    }
    if (_config["gateway"] != NULL && _config["gateway"] != ""){
      _ip = _config["gateway"];
      if (_valIP.fromString(_ip)) this->gateway.fromString(_ip);
      else return false;
    }
    if (_config["subnet"] != NULL && _config["subnet"] != ""){
      _ip = _config["subnet"];
      if (_valIP.fromString(_ip)) this->subnet.fromString(_ip);
      else return false;
    }
    if (_config["ssid"] == NULL || _config["ssid"] == "") return false;
    this->ssid = _config["ssid"];
    if (_config["password"] != NULL) this->password = _config["password"];
    if (_config["channel"] != NULL && (_config["channel"] >= 1 && _config["channel"] <= 13))
      this->channel = _config["channel"];
    if (_config["hidden"] != NULL && (_config["hidden"] == true || _config["hidden"] == false))
      this->hidden = _config["hidden"];
    return true;
  }
  else return false;
};
bool APControl::startAP(){
  if (this->valid()){
    if (!WiFi.softAPConfig(this->localIP, this->gateway, this->subnet)) return false;
    return WiFi.softAP(this->ssid, this->password, this->channel, this->hidden ? 1 : 0);
  }
  else return false;
};
bool APControl::stopAP(){
  return WiFi.softAPdisconnect(true);
};
const char* APControl::getSSID(){ return this->ssid; };
const char* APControl::getPassword(){ return this->password; };
int APControl::getChannel(){ return this->channel; };
bool APControl::getHidden(){ return this->hidden; };
