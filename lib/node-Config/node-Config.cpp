#include <node-Config.h>

const char* nConfig::path(){
  String _path = this->_module;
  _path = "config-"+_path+".json";
  return _path.c_str();
};

bool nConfig::load(){
  const char* _path = this->path();
  SPIFFS.begin();
  if (SPIFFS.exists(_path)){
    auto _file = SPIFFS.open(_path, "r");
    if (_file){
      DynamicJsonBuffer _buffer(512);
      JsonObject& _config = _buffer.parseObject(_file);
      if (_config["module"] && _config["module"] == this->_module){
        this->_config = &_config;
        this->_exists = true;
      }
      else this->_exists = false;
    }
    else this->_exists = false;
  }
  else this->_exists = false;
  return this->_exists;
};

bool nConfig::save(){
  if (this->_config == NULL){
    DynamicJsonBuffer _buffer(512);
    this->_config = _buffer.createObject();
    this->_config["module"] = this->_module;
  }
  if (this->_config["module"] == NULL || this->_config["module"] != this->_module) return false;
  const char* _path = this->path();
  auto _file = SPIFFS.open(_path, "w");
  int _saved = this->_config.printTo(_file);
  return (_saved > 0);
};

bool nConfig::remove(){
  const char* _path = this->path();
  if (SPIFFS.exists(_path)) return SPIFFS.remove(_path);
  else return true;
};

template<typename T>
void nConfig::set<T>(const char* key, T value){
  this->_config[key] = value;
};
