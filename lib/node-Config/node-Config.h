#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

class nConfig{
private:
  bool _exists;
  const char* _module;
  JsonObject& _config;
public:
  nConfig(const char* module);
  const char* path();
  bool load();
  bool save();
  bool remove();
  template<typename T>
  T get(const char* key);
  template<typename T>
  void set(const char* key, T value);
};

nConfig* loadConfig(const char* module);
