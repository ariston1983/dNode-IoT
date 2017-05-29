#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

void prepFS();
bool writeFile(String path, String content);
String readFile(String path);

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
  bool has(const char* key);
  template<typename TCompare>
  bool isEqual(const char* key, TCompare equal){
    if (this->has(key)) return this->_config[key] == equal;
    else return false;
  };
  template<typename TReturn>
  TReturn get(const char* key){
    TReturn _ret;
    if (this->_exist && key != "") _ret = this->_config[key];
    return _ret;
  };
  template<typename TValue>
  void set(const char* key, TValue value){
    Serial.print("Set config key: "); Serial.println(key);
    Serial.print("With value: "); Serial.println(value);
    if (this->_exist && key != "") this->_config[key] = value;
  };
  String toString();
};

nConfig* loadConfig(const char* module);
