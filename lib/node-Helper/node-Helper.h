#ifndef __NODE_HELPER__
#define __NODE_HELPER__

#include <string>
#include "Arduino.h"
#include "FS.h"
#include "ArduinoJson.h"

template <typename T_ty> struct TypeInfo{
  static const char * name;
  typedef T_ty base;
};
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";

#define TYPE_NAME(var) TypeInfo< typeof(var) >::name
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;

template<typename TLogReturn>
TLogReturn doLog(std::string message, TLogReturn ret){
  #if __NODE_DEBUG__
  Serial.println(message);
  #endif
  return ret;
};
void doLog(std::string message){
  #if __NODE_DEBUG__
  Serial.println(message);
  #endif
};

void prepFS(bool format = false){
  if (!SPIFFS.exists("/fs.info") || format){
    doLog("FS: Formatting");
    SPIFFS.format();
    File _file = SPIFFS.open("/fs.info", "w");
    _file.println("FS ready indicator");
    _file.close();
    doLog("FS: Format complete");
  }
};
bool fileExists(std::string path){
  doLog("FS: Checking path");
  prepFS();
  return SPIFFS.exists(path.c_str());
};
bool writeFile(std::string path, std::string content){
  prepFS();
  doLog("FS: Writting file");
  File _file = SPIFFS.open(path.c_str(), "w");
  bool _res = _file.println(content.c_str());
  _file.close();
  doLog("FS: Write complete");
  return _res;
};
std::string readFile(std::string path){
  std::string _content = "";
  if (fileExists(path)){
    doLog("FS: Loading file");
    File _file = SPIFFS.open(path.c_str(), "r");
    _content += _file.readString().c_str();
    _file.close();
    doLog("FS: Load complete");
  }
  return _content;
};
std::string stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  return static_cast<std::string>(_store);
};

#endif
