#include "Arduino.h"
//#include <node-Query.h>
//#include <node-Config.h>
#include <node-Factory.h>
#include <node-Module.h>
#include <node-WiFi.h>

class ModA: public nodeModule{
public:
  inline ModA(): nodeModule(){};
  inline virtual bool setup(){ Serial.println("ModA start"); return true; };
};

//ModuleFactory _fact;

void setup(){
  Serial.begin(115200);
  Serial.println();

  ModuleFactory::add<ModA>("ModA");
  //ModuleFactory::add<nodeAP>("nodeAP");
  // nodeModule* _mod = ModuleFactory::create("ModA");
  // _mod->start();

  Serial.println();
  Serial.println("Prep complete...");
}
void loop(){}
