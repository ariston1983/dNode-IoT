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

bool init(){
  this->_config = new APConfig();
  if (this->_config->load()){
    if (this->_config->isValid()){
      WiFi.softAPdisconnect();
      WiFi.softAPConfig(this->_config->getLocalIPA(), this->_config->getGatewayIPA(), this->_config->getSubnetIPA());
      return WiFi.softAP(this->_config->getSSID(), this->_config->getPassword(), this->_config->getChannel(), this->_config->getHidden() ? 1 : 0);
    }
    else return doLog<bool>("Invalid saved AP config", false);
  }
  else return doLog<bool>("Unable load AP config", false);
};
std::string nodeAP::execute(nodeQuery *query){ return ""; };
bool nodeAP::loop(){ return false; };

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

bool nodeSTA::loadConfig(){
  this->_config = new STAConfig();
  return this->_config->load();
};
std::string nodeSTA::execute(nodeQuery *query){ return ""; };
bool nodeSTA::loop(){ return false; };
