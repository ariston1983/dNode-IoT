#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

void prepFS(bool format = false);
bool writeFile(String path, String content);
String readFile(String path);

class nodeConfig{
private:
  String fsPath();
protected:
  String _module;
  JsonVariant _storage;
  void init();
  virtual void reset();
public:
  nodeConfig(String module);
  bool load();
  bool save();
  bool parseString(String json);
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
  template <typename TCompare>
  bool isEqual(String key, TCompare value){
    return this->_storage[key] == value;
  };
  String toString();
};

nodeConfig* loadConfig(String module);
