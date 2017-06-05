#include <node-Config.h>

void prepFS(){
  if (!SPIFFS.exists("/fs.info")){
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

void nodeConfig::reset(){
  DynamicJsonBuffer _buffer(512);
  this->_storage = _buffer.createObject();
  this->_storage["module"] = this->_module;
};
String nodeConfig::fsPath(){
  String _path = "/config-"+this->_module+".json";
  return _path;
};
nodeConfig::nodeConfig(String module){
  this->_module = module;
  this->reset();
};
bool nodeConfig::load(){
  String _fsContent = readFile(this->fsPath());
  bool _res = this->parseString(_fsContent);
  this->set<String>("module", this->_module);
  return _res;
};
bool nodeConfig::save(){
  writeFile(this->fsPath(), this->toString());
  return true;
};
bool nodeConfig::parseString(String json){
  if (json != NULL && json != ""){
    DynamicJsonBuffer _buffer(json.length());
    this->_storage = _buffer.parseObject(json);
    return true;
  }
  else{
    this->reset();
    return false;
  }
}
String nodeConfig::toString(){
  String _json;
  this->_storage.printTo(_json);
  return _json;
};

nodeConfig* loadConfig(String module){
  if (module != NULL && module != "") return new nodeConfig(module);
  else return NULL;
};
