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
    }

    HTTPClient http;
    String url = ENDERECOSERVIDOR + montaDiretorio(dado);
    boolean a = http.begin(url.c_str());
    http.GET();
    String resposta = http.getString();
    http.end();

    if (TUtilidadesParaStrings::pegaEntreAspas(resposta, 1).equalsIgnoreCase("OK"))
    {
      Serial.println("Dados enviados com sucesso.");
      return true;
    }
    return false;
  }

  ~TComunicacaoServidorHTTP()
  {
  }
};

#endif
