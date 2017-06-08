#include "Arduino.h"
#include "FS.h"
#include <node-WiFi.h>

class Parent{
protected:
  const char* _storage;
  JsonObject& createJsonEngine();
  void storeJson(JsonObject& json);
public:
  Parent();
  virtual void set();
  inline void show(){ Serial.println(this->_storage); };
};
Parent::Parent(){
  this->_storage = "";
};
JsonObject& Parent::createJsonEngine(){
  Serial.println("Create json object");
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.parseObject(this->_storage);
  _obj["base"] = "json";
  _obj.printTo(Serial);
  return _obj;
};
void Parent::storeJson(JsonObject& json){
  Serial.println("Store json into storage");
  json.printTo(Serial);
  char _store[json.measureLength()+1];
  json.printTo(_store, sizeof(_store));
  Serial.println(_store);
  this->_storage = _store;
};
void Parent::set(){
  JsonObject& _obj = this->createJsonEngine();
  _obj["parent"] = "parent";
  _obj.printTo(Serial);
  this->storeJson(_obj);
};
class Child: public Parent{
public:
  inline Child(): Parent(){};
  void set();
};
void Child::set(){
  JsonObject& _obj = this->createJsonEngine();
  _obj["child"] = "child";
  Serial.println("Current child object");
  _obj.printTo(Serial);
  this->storeJson(_obj);
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  Child _ch;
  _ch.set();
  _ch.show();

  //prepFS(true);

  //Serial.println("Create AP Config");
  //nodeAPConfig _config = new APConfig();
  //_config->save();
  //if (_config->load()) Serial.println(_config->toString());
  //else Serial.println("fail save/load config");
  //Serial.println(_config->toString());

  //nodeAPConfig _apConfig = new APConfig();
  //if (!_apConfig->load()) _apConfig = APConfig::defaultConfig();
  //Serial.println(_apConfig->toString());

  //APControl* _ap = new APControl();
  //APControl* _ap = new APControl("dNode-AP", "dNode-AP", 6, false);
  //_ap.saveConfig();

  // nConfig* _apConf = loadConfig("wifi-AP");
  // Serial.println(_apConf->toString());
  // if (_apConf->has("localIP")) Serial.println("Has LocalIP config");
  // else Serial.println("No LocalIP config");

  // SPIFFS.begin();
  //
  // nConfig* _conf = loadConfig("dummy");
  // if (!_conf->has("key2")) //Serial.println("no key2 config");
  //   _conf->set<const char*>("key2", "dodol");
  // Serial.println(_conf->toString());

  Serial.println("Prep complete...");
}
void loop(){}
