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

class configElement{
protected:
  JsonVariant _storage;

  template <typename TSetValue>
  void set(String key, TSetValue value){
    if (key != "") this->_storage[key] = value;
  };
  template <typename TGetValue>
  TGetValue get(String key){
    TGetValue _ret;
    if (key != "") _ret = this->_storage[key];
    return _ret;
  };
  bool copyTo(JsonVariant obj, String key);
public:
  String toString();
};
class nodeConfig : public configElement{
private:
  String _module;
  String fsPath();
public:
  nodeConfig(String module);
  bool load();
  bool save();
};
