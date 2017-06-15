#ifndef __NODE_QUERY__
#define __NODE_QUERY__

#include "Arduino.h"
#include <node-Helper.h>

class nodeQuery{
private:
  const char* _module;
  const char* _subs;
  const char* _command;
  const char* _args;
public:
  nodeQuery(const char* module, const char* subs, const char* command, const char* args);
  inline const char* getModule(){ return this->_module; };
  inline const char* getSubModule(){ return this->_subs; };
  inline const char* getCommand(){ return this->_command; };
  inline const char* getArguments(){ return this->_args; };
  inline bool hasArguments(){ return this->_args != NULL && this->_args != ""; };
  String toString();
};

#endif
