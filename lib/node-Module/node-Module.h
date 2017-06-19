#ifndef __NODE_MODULE__
#define __NODE_MODULE__

#include <map>
#include <string>
#include <node-Helper.h>
#include <node-Config.h>
#include <node-Query.h>

class nodeModule{
public:
  inline nodeModule(){ };
  inline nodeConfig* getConfig(){ return NULL; };
  inline virtual bool init(){ return doLog<bool>("Init not implemented", false); };
  inline virtual std::string execute(nodeQuery *query){ return doLog<std::string>("Execute not implemented", ""); };
  inline virtual bool loop(){ return doLog<bool>("Loop not implemented", false); };
};

#endif
