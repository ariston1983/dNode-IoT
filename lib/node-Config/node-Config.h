#ifndef __NODE_CONFIG__
#define __NODE_CONFIG__

#include <node-Helper.h>
#include <node-Query.h>

class nodeConfig{
private:
  std::string fsPath(){
    std::string _path = "/config-"+this->_module+".json";
    return _path;
  };
protected:
  std::string _module;
  virtual bool parseString(std::string toParse){
    if (toParse != ""){
      DynamicJsonBuffer _buffer(toParse.length());
      JsonVariant _var = _buffer.parseObject(toParse.c_str());
      if (!_var.is<JsonObject>()) return doLog<bool>("nodeConfig: invalid saved config", false);
      JsonObject& _obj = _var.as<JsonObject>();
      if (_obj["module"] != this->_module.c_str()) return doLog<bool>("nodeConfig: invalid config module", false);
      return doLog<bool>("nodeConfig: Parse complete", true);
    }
    else return doLog<bool>("nodeConfig: Empty string to parse", false);
  };
  inline virtual void defaultConfig(){ doLog("nodeConfig: defaultConfig not implemented"); };
  void init(){
    if (!this->exists()) this->defaultConfig();
    else this->load();
  };
public:
  inline nodeConfig(std::string module){ this->_module = module; };
  inline virtual std::string execute(nodeQuery* query){ doLog<std::string>("Query not implemented", ""); };
  inline bool exists(){ return fileExists(this->fsPath()); };
  bool load(){
    std::string _fsContent = readFile(this->fsPath());
    return this->parseString(_fsContent);
  };
  inline bool save(){ return writeFile(this->fsPath(), this->toString()); };
  virtual std::string toString(){
    DynamicJsonBuffer _buffer(512);
    JsonVariant _var = _buffer.createObject();
    JsonObject& _obj = _var.as<JsonObject>();
    _obj["module"] = this->_module.c_str();
    return stringify(_obj);
  };
};

#endif
