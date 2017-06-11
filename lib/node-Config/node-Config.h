#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

void prepFS(bool format = false);
bool writeFile(String path, String content);
String readFile(String path);
String stringify(JsonVariant json);

class nodeConfig{
private:
  String fsPath();
protected:
  String _module;
public:
  nodeConfig(String module);
  bool load();
  bool save();
  bool parseString(String json);
  virtual String toString();
};

nodeConfig* loadConfig(String module);
