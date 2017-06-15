#include <node-Config.h>

String nodeConfig::fsPath(){
  String _path = "/config-"+this->_module+".json";
  return _path;
};
void nodeConfig::defaultConfig(){ Serial.println("defaultConfig not implemented"); };
void nodeConfig::init(){
  if (!this->exists()) this->defaultConfig();
  else this->load();
};
nodeConfig::nodeConfig(String module){
  this->_module = module;
};
bool nodeConfig::exists(){
  Serial.println("Check existing config");
  return fileExists(this->fsPath());
};
bool nodeConfig::load(){
  String _fsContent = readFile(this->fsPath());
  return this->parseString(_fsContent);
};
bool nodeConfig::save(){
  writeFile(this->fsPath(), this->toString());
  return true;
};
bool nodeConfig::parseString(String toParse){
  if (toParse != NULL && toParse != ""){
    DynamicJsonBuffer _buffer(toParse.length());
    JsonVariant _var = _buffer.parseObject(toParse.c_str());
    if (!_var.is<JsonObject>()) return false;
    JsonObject& _obj = _var.as<JsonObject>();
    if (_obj["module"] != this->_module.c_str()) return false;
    return true;
  }
  else return false;
}
String nodeConfig::toString(){
  DynamicJsonBuffer _buffer(512);
  JsonVariant _var = _buffer.createObject();
  JsonObject& _obj = _var.as<JsonObject>();
  _obj["module"] = this->_module.c_str();
  return stringify(_obj);
};
