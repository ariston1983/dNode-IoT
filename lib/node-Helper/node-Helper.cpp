#include <node-Helper.h>

void prepFS(bool format){
  if (!SPIFFS.exists("/fs.info") || format){
    Serial.println("Formatting");
    SPIFFS.format();
    File _file = SPIFFS.open("/fs.info", "w");
    _file.println("FS ready indicator");
    _file.close();
    Serial.println("Format complete");
  }
};
bool fileExists(String path){
  Serial.println("Checking file");
  prepFS();
  return SPIFFS.exists(path.c_str());
};
bool writeFile(String path, String content){
  prepFS();
  Serial.println("Writting file");
  File _file = SPIFFS.open(path.c_str(), "w");
  bool _res = _file.println(content.c_str());
  _file.close();
  Serial.println("Write complete");
  return _res;
};
String readFile(String path){
  String _content = "";
  if (fileExists(path)){
    Serial.println("Loading file");
    File _file = SPIFFS.open(path.c_str(), "r");
    _content += _file.readString();
    _file.close();
    Serial.println("Load complete");
  }
  return _content;
};
String stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  String _str = _store;
  return _str;
};
