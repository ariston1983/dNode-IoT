#include "Arduino.h"
#include <node-Config.h>

class nodeSubscription{
public:
  nodeSubscription(String topic);
  String topic();
};
typedef nodeSubscription* subscription;

class publishData{
public:
  String toString();
};
typedef publishData* pubData;

class nodePublish{
private:
  String _topic;
  pubData _data;
public:
  nodePublish(String topic, pubData data = NULL);
  String topic();
  String message();
};
typedef nodePublish* publish;

class nodeMQTTConfig : public nodeConfig{
public:
  nodeMQTTConfig();
  bool isValid();
};
typedef nodeMQTTConfig* mqttConfig;

class nodeMQTT{
private:
  mqttConfig _config;
public:
  nodeMQTT(mqttConfig config);
  bool setup();
  void addSubscription(subscription sub);
  void removeSubscription(subscription sub);
  void publish(publish pub);
};
