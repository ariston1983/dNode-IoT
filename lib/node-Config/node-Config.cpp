#include <node-Config.h>

void prepFS(bool format){
  if (!SPIFFS.exists("/fs.info") || format){
    SPIFFS.format();
    File _file = SPIFFS.open("/fs.info", "w");
    _file.println("FS ready indicator");
    _file.close();
  }
};
bool writeFile(String path, String content){
  prepFS();
  File _file = SPIFFS.open(path.c_str(), "w");
  bool _res = _file.println(content.c_str());
  _file.close();
  return _res;
};
String readFile(String path){
  prepFS();
  String _content = "";
  if (SPIFFS.exists(path.c_str())){
    File _file = SPIFFS.open(path.c_str(), "r");
    _content += _file.readString();
    _file.close();
  }
  return _content;
};
String stringify(JsonVariant json){
  String _str = "";
  if (json){
    int _len = json.measureLength();
    char _store[_len];
    json.printTo(_store, _len);
    _str += _store;
  }
  return _str;
};

String nodeConfig::fsPath(){
  String _path = "/config-"+this->_module+".json";
  return _path;
};
nodeConfig::nodeConfig(String module){
  this->_module = module;
};
bool nodeConfig::load(){
  String _fsContent = readFile(this->fsPath());
  return this->parseString(_fsContent);
};
bool nodeConfig::save(){
  return writeFile(this->fsPath(), this->toString());
};
bool nodeConfig::parseString(String json){
  if (json != NULL && json != ""){
    DynamicJsonBuffer _buffer(json.length());
    JsonVariant _var = _buffer.parseObject(json);
    if (!_var.is<JsonObject>()) return false;
    JsonObject& _obj = _var.as<JsonObject>();
    if (_obj["module"] != this->_module.c_str()) return false;
    return true;
  }
  else return false;
}
String nodeConfig::toString(){
  Serial.println("json stringify");
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _obj["module"] = this->_module.c_str();
  return stringify(_obj);
};

nodeConfig* loadConfig(String module){
  if (module != NULL && module != "") return new nodeConfig(module);
  else return NULL;
};
