#include <node-Config.h>

void prepFS(bool format){
  if (!SPIFFS.exists("/fs.info") || format){
    SPIFFS.format();
    File _file = SPIFFS.open("/fs.info", "w");
    _file.println("FS ready indicator");
    _file.close();
  }
};
bool fileExists(String path){
  prepFS();
  return SPIFFS.exists(path.c_str());
};
bool writeFile(String path, String content){
  prepFS();
  File _file = SPIFFS.open(path.c_str(), "w");
  bool _res = _file.println(content.c_str());
  _file.close();
  return _res;
};
String readFile(String path){
  String _content = "";
  if (fileExists(path)){
    File _file = SPIFFS.open(path.c_str(), "r");
    _content += _file.readString();
    _file.close();
  }
  return _content;
};
String stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  String _str = _store;
  return _str;
};

String nodeConfig::fsPath(){
  String _path = "/config-"+this->_module+".json";
  return _path;
};
void nodeConfig::defaultConfig(){ };
nodeConfig::nodeConfig(String module){
  this->_module = module;
  if (!this->exists()) this->defaultConfig();
};
bool nodeConfig::exists(){
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
