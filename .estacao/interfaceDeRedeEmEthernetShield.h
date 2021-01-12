#ifndef TINTERFACEDEREDEEMETHERNETSHIELD
#define TINTERFACEDEREDEEMETHERNETSHIELD
#include <Arduino.h>
#include <string.h>

class TInterfaceDeRedeEmEthernetShield : public TInterfaceDeRede
{

private:
  EthernetClient ethernetClient;
  byte *mac;

public:
  TInterfaceDeRedeEmEthernetShield(byte *mac)
  {
    this->mac = mac;
  }

  boolean conexaoViaDHCP()
  {
    boolean sucesso = Ethernet.begin(this->mac, 10000, 4000);
    delay(1000);
    return sucesso;
  }

  String fazRequisicaoHTTP(IPAddress servidor, const String &diretorio)
  {

    String temp = "FAZENDO REQUISICAO HTTP PARA \"";
    temp.concat(diretorio);
    temp.concat("\"");

    ethernetClient.setTimeout(300);

    int vc = 0;
    while (vc < TENTATIVASSERVIDORHTTP && !ethernetClient.connect(servidor, 80))
    {
      vc++;
    }

    if (vc == TENTATIVASSERVIDORHTTP)
    {
      ethernetClient.stop();
      return "";
    }

    ethernetClient.println("GET " + diretorio + " HTTP/1.1");
    ethernetClient.println("Connection: close");
    ethernetClient.println();

    String resposta = "";
    delay(100);
    unsigned long int inicioRequisicao = now();
    while ((ethernetClient.available() || ethernetClient.connected()) && ((now() - inicioRequisicao) < SEGUNDOSANTESDEABORTAR))
    {
      if (ethernetClient.available())
      {
        resposta.concat((char)ethernetClient.read());
      }
    }
    ethernetClient.stop();

    String temp2 = "RESPOSTA DA REQUISICAO HTTP: \"";
    temp2.concat(resposta);
    temp2.concat("\"");
    TLog::envia(temp2.c_str());

    return resposta;
  }

  String fazRequisicaoHTTP(const char *servidor, const String &diretorio)
  {

    String temp = "FAZENDO REQUISICAO HTTP PARA \"";
    temp.concat(diretorio);
    temp.concat("\"");
    TLog::envia(temp.c_str());

    int vc = 0;
    while (vc < TENTATIVASSERVIDORHTTP && !ethernetClient.connect(servidor, 80))
    {
      vc++;
    }

    if (vc == TENTATIVASSERVIDORHTTP)
    {
      ethernetClient.stop();
      return "";
    }

    ethernetClient.println("GET " + diretorio + " HTTP/1.1");
    ethernetClient.print("Host: ");
    ethernetClient.println(servidor);
    ethernetClient.println("Connection: close");
    ethernetClient.println();

    String resposta = "";
    delay(100);
    unsigned long int inicioRequisicao = now();
    while ((ethernetClient.available() || ethernetClient.connected()) && ((now() - inicioRequisicao) < SEGUNDOSANTESDEABORTAR))
    {
      if (ethernetClient.available())
      {
        resposta.concat((char)ethernetClient.read());
      }
    }
    ethernetClient.stop();

    String temp2 = "RESPOSTA DA REQUISICAO HTTP: \"";
    temp2.concat(resposta);
    temp2.concat("\"");
    TLog::envia(temp2.c_str());

    return resposta;
  }

  IPAddress getIP()
  {
    return Ethernet.localIP();
  }

  IPAddress getGateway()
  {
    return Ethernet.gatewayIP();
  }
};

#endif
