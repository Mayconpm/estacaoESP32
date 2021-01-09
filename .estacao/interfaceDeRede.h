#ifndef TINTERFACEDEREDE
#define TINTERFACEDEREDE
#include <Arduino.h>
#include <string.h>

class TInterfaceDeRede {

public:
  
  virtual boolean conexaoViaDHCP();
  
  virtual String fazRequisicaoHTTP(IPAddress servidor, const String & diretorio);

  virtual String fazRequisicaoHTTP(const char* servidor, const String & diretorio);
  
  virtual IPAddress getIP();
  
  virtual IPAddress getGateway();
  
};

#endif



