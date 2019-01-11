#ifndef setupMQTT_h

#define setupMQTT_h

class LedManager{
  public:
    LedManager();
    void SETUP();
    void ONLED();
    void OFFLED();
};

extern LedManager ledManager;
#endif
