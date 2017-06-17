#ifndef __NODE_HELPER__
#define __NODE_HELPER__

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

void prepFS(bool format = false);
bool fileExists(String path);
bool writeFile(String path, String content);
String readFile(String path);
String stringify(JsonVariant json);

#endif
