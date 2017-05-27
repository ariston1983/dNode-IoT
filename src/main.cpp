#include "Arduino.h"
#include "FS.h"
#include <node-Config.h>

template <typename T>
void f(T value){
  Serial.print("testing template, value: ");
  Serial.println(value);
};

void setup(){
  Serial.begin(115200);

  f<int>(100);
  f<const char*>("Hello");
  nConfig* _config = new nConfig("testing");
  _config->set("key1", "hello");
  _config->save();

  _config = new nConfig("testing");
  if (_config->load()){
    const char* value = _config->get<const char*>("key1");
    Serial.print("key1 value: ");
    Serial.println(value);
  }
  else Serial.println("unable load config");

  Serial.println("Prep complete...");
}
void loop(){}
