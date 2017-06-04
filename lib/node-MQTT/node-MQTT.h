#include "Arduino.h";

class nodeMQTT{
private:
public:
  bool setup();
  void subscribe();
  void publish(pubObject &value);
};

class pubObject{
public:
  String toJSON();
};
