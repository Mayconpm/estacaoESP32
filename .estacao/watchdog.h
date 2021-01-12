#ifndef WATCHDOGCLASS
#define WATCHDOGCLASS
#include <Arduino.h>

class TWatchdog {
  
private:
  int valor;
  int tempo;

public:
  
  //Construtor:
  TWatchdog(const int  & tempo) {
    this->tempo = tempo - 7;
    this->valor = 0;
    wdt_enable(WDTO_8S);
  }

  void reset() {
    this->valor = 0;
  }
  
  void interrupcao() {
    this->valor++;
    if (this->valor < this->tempo) {
      wdt_reset();
    }
  }
  
  //Destrutor:
  ~TWatchdog() {
  }
};

#endif
