#include "Arduino.h"
#include "FS.h"
#include <node-Config.h>

void setup(){
  Serial.begin(115200);

  //SPIFFS.begin();
  //File _file = SPIFFS.open("/test.txt", "r");
  //String _content = _file.readString();
  //Serial.println(_content);
  //_file.close();

  //Dir _dir = SPIFFS.openDir("/");
  //while (_dir.next()){
  //  Serial.println(_dir.fileName());
  //  Serial.println(_dir.fileSize());
  //  File _file = _dir.openFile("r");
  //  //String _content = _file.readStringUntil('\n');
  //  String _content = _file.readString();
  //  Serial.println(_content);
  //}

  //fileWrite("/test.txt", "some text here, dummy");
  //String _read = fileRead("/test.txt");
  //Serial.println(_read);

  //nConfig* _config = new nConfig("testing");
  //_config->set("key1", "hello");
  //if (_config->save()) Serial.println("config saved");
  //else Serial.println("config not saved");

  //_config = new nConfig("testing");
  //if (_config->load()){
  //  const char* value = _config->get<const char*>("key1");
  //  Serial.print("key1 value: ");
  //  Serial.println(value);
  //}
  //else Serial.println("unable load config");

  Serial.println("Prep complete...");
}
void loop(){}
