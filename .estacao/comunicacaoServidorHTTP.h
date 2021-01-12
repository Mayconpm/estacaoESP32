#ifndef TCOMUNICACAOSERVIDORHTTP
#define TCOMUNICACAOSERVIDORHTTP
#include <Arduino.h>
#include <string.h>
#include "interfaceDeRedeEmEthernetShield.h"
#include "utilidadesParaStrings.h"

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

  void conectaViaDHCP()
  {
    this->conectado = false;
    TLog::envia("TENTANDO CONEXAO VIA DHCP...");
    int tentativa = 1;
    while (tentativa <= TENTATIVASDHCP)
    {
      String temp = "TENTANDO CONEXAO VIA DHCP... [";
      temp.concat(tentativa);
      temp.concat("]");
      TLog::envia(temp.c_str());
      if (interfaceDeRede->conexaoViaDHCP())
      {
        this->conectado = true;
        break;
      }
      tentativa++;
    }
    if (this->conectado)
    {
      TLog::envia("CONEXAO VIA DHCP ESTABELECIDA COM SUCESSO");

      IPAddress IP = interfaceDeRede->getIP();
      TLog::envia("IP: %d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);

      IPAddress gateway = interfaceDeRede->getGateway();
      TLog::envia("GATEWAY: %d.%d.%d.%d", gateway[0], gateway[1], gateway[2], gateway[3]);
    }
    else
    {
      TLog::envia("FALHA NA CONEXAO VIA DHCP");
    }
  }

public:
  TComunicacaoServidorHTTP(byte *mac, String chave)
  {
    this->chave = chave;

    this->interfaceDeRede = new TInterfaceDeRedeEmEthernetShield(mac);

    this->conectaViaDHCP();
  }

  boolean enviar(const TDado &dado)
  {
    if (!this->conectado)
    {
      this->conectaViaDHCP();
      return false;
    }
    String resposta = this->interfaceDeRede->fazRequisicaoHTTP(ENDERECOSERVIDOR, montaDiretorio(dado));
    if (TUtilidadesParaStrings::pegaEntreAspas(resposta, 1).equalsIgnoreCase("OK"))
    {
      return true;
    }
    return false;
  }

  ~TComunicacaoServidorHTTP() {}
};

#endif
