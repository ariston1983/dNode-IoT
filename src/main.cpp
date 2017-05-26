#include "Arduino.h"
#include "FS.h"
#include <node-Config.h>

void setup(){
  Serial.begin(115200);

  nConfig* _config = new nConfig("testing");
  _config->set<const char*>("key1", "hello");
  _config->save();

  Serial.println("Prep complete...");
}
void loop(){}
