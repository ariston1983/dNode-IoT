#include "Arduino.h"
#include <node-Query.h>
#include <node-Config.h>
#include <node-Module.h>
#include <node-WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();

  nodeQuery* _query = new nodeQuery(
      "node-AP"
    , "config"
    , "set"
    , "{'ssid':'dodol', 'password': 'dodol', 'channel': 6, 'hidden': false, 'localIP': '192.168.1.1', 'gateway': '192.168.1.1', 'subnet': '255.255.255.0'}"
  );
  Serial.println("query");
  Serial.println(_query->toString());

  nodeAPConfig _config = new APConfig();
  Serial.println("config");
  Serial.println(_config->toString());

  String _res = _config->query(_query);
  Serial.println(_res);

  Serial.println();
  Serial.println("Prep complete...");
}
void loop(){}
