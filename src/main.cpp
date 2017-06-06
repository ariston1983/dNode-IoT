#include "Arduino.h"
#include "FS.h"
#include <node-WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();

  prepFS(true);

  //Serial.println("Create AP Config");
  nodeAPConfig _config = new APConfig();
  //_config->save();
  //if (_config->load()) Serial.println(_config->toString());
  //else Serial.println("fail save/load config");
  Serial.println(_config->toString());

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
