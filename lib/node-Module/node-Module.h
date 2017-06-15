#ifndef __NODE_MODULE__
#define __NODE_MODULE__

#include <node-Config.h>
#include <node-Query.h>

class nodeModule{
protected:
  bool _running;
  inline virtual bool loadConfig(){ return false; };
public:
  nodeModule();
  inline bool isRunning(){ return this->_running; };
  virtual nodeConfig& getConfig();
  inline virtual const char* query(nodeQuery *query){ Serial.println("Query not implementd"); return ""; };
  inline virtual bool start(){ Serial.println("Start not implementd"); return false; };
  inline virtual bool pause(){ Serial.println("Pause not implementd"); return false; };
  inline virtual bool stop(){ Serial.println("Stop not implementd"); return false; };
};

#endif
