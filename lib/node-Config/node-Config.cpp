#include <node-Config.h>

nConfig::nConfig(const char* module){};

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

};

bool nConfig::remove(){
  String _path = this->path();
  if (SPIFFS.exists(_path.c_str())) return SPIFFS.remove(_path.c_str());
  else return true;
};
