#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

class nConfig{
private:
  bool _exists;
  const char* _module;
  JsonVariant _config;
public:
  nConfig(const char* module);
  const char* path();
  bool load();
  bool save();
  bool remove();
  template<typename TReturn>
  TReturn get(const char* key){
    if (this->_exists && key != "") return this->_config[key];
    else return NULL;
  };
  template<typename TValue>
  void set(const char* key, TValue value){
    if (this->_exists && key != "") this->_config[key] = value;
  };
};

nConfig* loadConfig(const char* module);
