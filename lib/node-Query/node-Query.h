#ifndef __NODE_QUERY__
#define __NODE_QUERY__

#include <map>
#include <string>
#include "Arduino.h"
#include <node-Helper.h>

class nodeQuery{
private:
  std::string _module;
  std::string _sub;
  std::string _command;
  std::string _args;
public:
  static nodeQuery* create(std::string module, std::string sub, std::string command, std::string args = ""){
    return new nodeQuery(module, sub, command, args);
  };
  nodeQuery(std::string module, std::string sub, std::string command, std::string args = ""){
    this->_module = module;
    this->_sub = sub;
    this->_command = command;
    this->_args = args;
  };
  inline std::string getModule(){ return this->_module; };
  inline std::string getSubModule(){ return this->_sub; };
  inline std::string getCommand(){ return this->_command; };
  inline std::string getArguments(){ return this->_args; };
  inline bool hasArguments(){ return !this->_args.empty() && this->_args != ""; };
  std::string toString(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    _obj["module"] = this->_module.c_str();
    _obj["sub"] = this->_sub.c_str();
    _obj["command"] = this->_command.c_str();
    _obj["args"] = this->_args.c_str();
    return stringify(_obj);
  };
};

class queryResponse{
  typedef std::map<std::string, std::string> data_Type;
private:
  nodeQuery* _query;
  int _code;
  std::string _message;
  data_Type* _data;
  data_Type* getMap(){
    if (this->_data == NULL) this->_data = new data_Type();
    return this->_data;
  };
public:
  static queryResponse* create(nodeQuery* query){ return new queryResponse(query); };
  queryResponse(nodeQuery* query, int code = 0, std::string message = "OK"){
    this->_query = query;
    this->_code = code;
    this->_message = message;
  };
  nodeQuery* getQuery(){ return this->_query; };
  queryResponse* changeCode(int code){ this->_code = code; return this; };
  queryResponse* changeMessage(std::string message){ this->_message = message; return this; };
  queryResponse* addData(std::string key, std::string value){
    if (!key.empty() && key != "")
      this->_data->insert(std::make_pair(key, value));
    return this;
  };
  std::string toString(int size = 512, bool inclQuery = false){
    DynamicJsonBuffer _buffer(size);
    JsonObject& _obj = _buffer.createObject();
    _obj["code"] = this->_code;
    _obj["message"] = this->_message.c_str();
    if (inclQuery)
      _obj["query"] = this->_query->toString().c_str();
    if (this->_data != NULL)
      for (data_Type::iterator _it = getMap()->begin(); _it != getMap()->end(); ++_it)
        _obj[_it->first.c_str()] = _it->second.c_str();
    return stringify(_obj);
  };
};

#endif
