#include <node-Query.h>

nodeQuery::nodeQuery(const char* module, const char* subs, const char* command, const char* args){
  this->_module = module;
  this->_subs = subs;
  this->_command = command;
  this->_args = args;
};
String nodeQuery::toString(){
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _obj["module"] = this->_module;
  _obj["sub-module"] = this->_subs;
  _obj["command"] = this->_command;
  _obj["args"] = this->_args;
  return stringify(_obj);
};
