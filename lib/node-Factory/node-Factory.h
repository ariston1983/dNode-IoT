#ifndef __NODE_FACTORY__
#define __NODE_FACTORY__

#include <string>
#include <map>
#include <node-Module.h>

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
  static void setup(){
    for (map_Module::iterator _it = getMap()->begin(); _it != getMap()->end(); ++_it){
      bool _res = _it->second()->setup();
      doLog("Setup: mod [" + _it->first + "] result: " + (_res ? "success" : "failed"));
    };
  };
  static void loop(){
    for (map_Module::iterator _it = getMap()->begin(); _it != getMap()->end(); ++_it){
      bool _res = _it->second()->loop();
      doLog("Loop: mod [" + _it->first + "] result: " + (_res ? "success" : "failed"));
    };
  };
  static std::string execute(nodeQuery* query){
    nodeModule* _mod = create(query->getModule());
    if (_mod != NULL) return _mod->execute(query);
    else doLog<std::string>("Execute: mod [" + query->getModule() + "] not found", "");
  };
};

#ifndef __NODE_FACT_INIT__
#define __NODE_FACT_INIT__
map_Module* ModuleFactory::_map = new map_Module();
#endif

#endif
