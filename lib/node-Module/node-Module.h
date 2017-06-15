class nodeModule{
protected:
  bool loadConfig();
public:
  nodeModule();
  virtual bool start();
  virtual bool pause();
  virtual bool stop();
};
