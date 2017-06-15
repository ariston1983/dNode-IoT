#ifndef __NODE_CONFIG__
#define __NODE_CONFIG__

#include <node-Helper.h>
#include <node-Query.h>

class nodeConfig{
private:
  String fsPath();
protected:
  String _module;
  virtual bool parseString(String toParse);
  virtual void defaultConfig();
  void init();
public:
  nodeConfig(String module);
  inline virtual String query(nodeQuery* query){ Serial.println("Query not implemented"); return ""; };
  bool exists();
  bool load();
  bool save();
  virtual String toString();
};

#endif
