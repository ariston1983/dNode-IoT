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

nConfig* loadConfig(const char* module){
    if (module != NULL && module != "")
      return new nConfig(module);
    else return NULL;
};

nConfig::nConfig(const char* module){
  this->_module = module;
  this->load();
  if (!this->_exist){
    DynamicJsonBuffer _buffer(512);
    this->_config = _buffer.createObject();
    this->_config["module"] = this->_module;
    this->_exist = true;
  }
};
String nConfig::path(){
  String _path = "/config-";
  _path += this->_module;
  _path += ".json";
  return _path;
};
bool nConfig::load(){
  String _path = this->path();
  String _content = readFile(_path);
  if (_content != ""){
    DynamicJsonBuffer _buffer(512);
    this->_config = _buffer.parse(_content);
    if (this->_config && this->_config["module"] == this->_module)
      this->_exist = true;
    else
      this->_exist = false;
  }
  else this->_exist = false;
};
bool nConfig::save(){
  this->_exist = writeFile(this->path(), this->toString());
  return this->_exist;
};
bool nConfig::remove(){
  String _path = this->path();
  if (SPIFFS.exists(_path.c_str())) return SPIFFS.remove(_path.c_str());
  else return true;
};
bool nConfig::has(const char* key){
  if (this->_exist){
    JsonObject& _json = this->_config;
    return _json.containsKey(key);
  }
  else return false;
}
String nConfig::toString(){
  if (!this->_exist){
    DynamicJsonBuffer _buffer(512);
    this->_config = _buffer.createObject();
    this->_config["module"] = this->_module;
  }
  String _json;
  this->_config.printTo(_json);
  return _json;
};
