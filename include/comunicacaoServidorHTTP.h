#ifndef TCOMUNICACAOSERVIDORHTTP
#define TCOMUNICACAOSERVIDORHTTP
#include <Arduino.h>
#include <string.h>
#include <HTTPClient.h>
#include "armazenamento.h"
#include "dado.h"
#include "interfaceWireless.h"
#include "utilidadesParaStrings.h"

class TComunicacaoServidorHTTP
{

private:
  String chave;
  boolean conectado;
  TinterfaceWireless interfaceWireless;

  const String montaURL(const TDado &dado)
  {
    String url = "/comm.php?chave=";
    url.concat(this->chave);
    url.concat("&tipo=1");
    url.concat("&valor=");
    String temp = "";
    temp.concat(dado.getValor());
    temp.trim();
    url.concat(temp);
    url.concat("&data=");
    url.concat(dado.getData());
    return url;
  }

public:
  TComunicacaoServidorHTTP(String chave)
  {
    this->chave = chave;
  }

  boolean enviar(const TDado &dado)
  {
    HTTPClient http;
    String url = ENDERECOSERVIDOR + montaURL(dado);
    Serial.println(url);
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode == 200)
    {
      Serial.println("Dados enviados com sucesso.");
      http.end();
      return true;
    }

    http.end();
    return false;
  }

  ~TComunicacaoServidorHTTP()
  {
  }
};

#endif
