#include "Arduino.h";
#include "<node-Config>"

class nodeMQTT{
private:
public:
  bool setup();
  void subscribe();
  void publish(pubObject &value);
};

class nodeMQTTConfig : public nodeConfig{

};

class pubObject{
public:
  String toJSON();
};
