#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

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

class nConfig{
private:
  bool _exist;
  const char* _module;
  JsonVariant _config;
  String path();
public:
  nConfig(const char* module);
  bool load();
  bool save();
  bool remove();
  template<typename TReturn>
  TReturn get(const char* key){
    if (this->_exist && key != "") return this->_config[key];
    else return NULL;
  };
  template<typename TValue>
  void set(const char* key, TValue value){
    if (this->_exist && key != "") this->_config[key] = value;
  };
};

nConfig* loadConfig(const char* module){
  if (module != NULL && module != "")
    return new nConfig(module);
  else return NULL;
};
