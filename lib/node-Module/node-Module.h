#ifndef __NODE_MODULE__
#define __NODE_MODULE__

#include <map>
#include <string>
#include <node-Config.h>
#include <node-Query.h>

class nodeModule{
protected:
  bool _running;
  inline virtual bool loadConfig(){ return false; };
public:
  inline nodeModule(){ };
  inline bool isRunning(){ return this->_running; };
  // virtual nodeConfig& getConfig();
  // inline virtual const char* query(nodeQuery *query){ Serial.println("Query not implementd"); return ""; };
  inline virtual bool start(){ Serial.println("Start not implementd"); return false; };
  inline virtual bool pause(){ Serial.println("Pause not implementd"); return false; };
  inline virtual bool stop(){ Serial.println("Stop not implementd"); return false; };
};

template<typename T>
nodeModule* createInstance(){ return new T(); };
typedef std::map<std::string, nodeModule*(*)()> map_Module;

struct ModuleFactory{
private:
  static map_Module* _map;
  static map_Module* getMap(){
    if (_map == NULL) _map = new map_Module();
    return _map;
  };
public:
  template<typename T>
  static void add(std::string const& module){
    getMap()->insert(std::make_pair(module, &createInstance<T>));
  };
  static nodeModule* create(std::string const& module){
    map_Module::iterator _it = getMap()->find(module);
    if (_it == getMap()->end()) return NULL;
    else return _it->second();
  };
  // static const char* execute(std::string const& module, nodeQuery* query){
  //   map_Module::iterator _it = getMap()->find(module);
  //   if (_it == getMap()->end()) return "";
  //   else return _it->second()->query(query);
  // };
};
map_Module* ModuleFactory::_map = new map_Module();

#endif
