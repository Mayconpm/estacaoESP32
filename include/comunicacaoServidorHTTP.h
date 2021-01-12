#ifndef TCOMUNICACAOSERVIDORHTTP
#define TCOMUNICACAOSERVIDORHTTP
#include <Arduino.h>
#include <string.h>
#include "armazenamento.h"
#include "dado.h"
#include "interfaceWireless.h"
#include "utilidadesParaStrings.h"
#include <HTTPClient.h>

class TComunicacaoServidorHTTP
{

private:
  String chave;
  boolean conectado;

  const String montaDiretorio(const TDado &dado)
  {
    String diretorio = "/comm.php?chave=";
    diretorio.concat(this->chave);
    diretorio.concat("&tipo=");
    diretorio.concat(dado.getTipo());
    diretorio.concat("&valor=");
    String temp = "";
    temp.concat(dado.getValor());
    temp.trim();
    diretorio.concat(temp);
    diretorio.concat("&data=");
    diretorio.concat(dado.getData());
    return diretorio;
  }

public:
  TComunicacaoServidorHTTP(String chave)
  {
    this->chave = chave;

    TinterfaceWireless::conectaWifi();
  }

  boolean enviar(const TDado &dado)
  {
    if (!this->conectado)
    {
      this->conectado = TinterfaceWireless::conectaWifi();
      return false;
      exit(0);
    }

    HTTPClient http;
    String url = ENDERECOSERVIDOR + montaDiretorio(dado);
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String resposta = http.getString();
      if (TUtilidadesParaStrings::pegaEntreAspas(resposta, 1).equalsIgnoreCase("OK"))
      {
        Serial.println("Dados enviados com sucesso.");
        return true;
      }
    }
    http.end();

    return false;
  }

  ~TComunicacaoServidorHTTP()
  {
  }
};

#endif
