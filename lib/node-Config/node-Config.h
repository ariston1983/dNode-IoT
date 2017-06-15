#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>

void prepFS(bool format = false);
bool fileExists(String path);
bool writeFile(String path, String content);
String readFile(String path);
String stringify(JsonVariant json);

class nodeConfig{
private:
  String fsPath();
protected:
  String _module;
  virtual bool parseString(String toParse);
  virtual void defaultConfig();
public:
  nodeConfig(String module);
  bool exists();
  bool load();
  bool save();
  String toString();
};
