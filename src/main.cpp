#include "Arduino.h"
#include "FS.h"
#include <node-WiFi.h>

JsonObject& createJson(String toParse = "{}", int size = 512){
  DynamicJsonBuffer _buffer(size);
  JsonVariant _obj = _buffer.parseObject(toParse);
  if (!_obj.is<JsonObject>()) _obj = _buffer.createObject();
  return _obj.as<JsonObject>();
};
String stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  return _store;
};

class Parent{
public:
  String _storage;
  Parent();
  virtual void set();
  inline void show(){ Serial.println(this->_storage); };
};
Parent::Parent(){
  this->_storage = "{}";
};
void Parent::set(){
  JsonObject& _obj = createJson(this->_storage);
  _obj["parent"] = "parent";
  this->_storage = stringify(_obj);
};
class Child: public Parent{
public:
  inline Child(): Parent(){};
  void set();
};
void Child::set(){
  Serial.println(this->_storage);
  JsonObject& _obj = createJson(this->_storage);
  _obj.printTo(Serial);
  _obj["child"] = "child";
  this->_storage = stringify(_obj);
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  //DynamicJsonBuffer _buff(512);
  //JsonObject& _obj = _buff.createObject();
  //JsonObject& _obj = createJson("{'dodol':'dodol'}");
  //_obj["dodol"] = "testing";
  //_obj.printTo(Serial);
  //String _st = stringify(_obj);
  //Serial.println(_st);

  //Parent _pr;
  //JsonVariant _obj = _pr.createJsonEngine();
  //_obj["test"] = "parent";
  //Serial.println(_obj.measureLength());
  //_pr.storeJson(_obj);
  //_pr.set();
  //_pr.show();
  Child _ch;
  _ch.set();
  _ch.show();
  //Serial.println(_ch._storage);

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

  Serial.println();
  Serial.println("Prep complete...");
}
void loop(){}
