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
  // static const char* execute(std::string const& module, nodeQuery* query){
  //   map_Module::iterator _it = getMap()->find(module);
  //   if (_it == getMap()->end()) return "";
  //   else return _it->second()->query(query);
  // };
};
map_Module* ModuleFactory::_map = new map_Module();

#endif
