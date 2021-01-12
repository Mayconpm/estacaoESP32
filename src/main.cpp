#include <Arduino.h>
#include "time.h"
#include "SPIFFS.h"
#include "configuracoes.h"
#include "armazenamento.h"
#include "comunicacaoServidorHTTP.h"
#include "interfaceWireless.h"
#include "sensorPluviometro.h"

TArmazenamento *armazenamento;
TComunicacaoServidorHTTP *comunicacaoServidorHTTP;
TinterfaceWireless *interfaceWireless;
TSensorPluviometro *sensorPluviometro;

void interrupcaoPluviometro()
{
  sensorPluviometro->interrupcao();
}

unsigned long int ultimaLeituraMenor;
unsigned long int ultimaLeituraMaior;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  armazenamento = new TArmazenamento();
  sensorPluviometro = new TSensorPluviometro();
  comunicacaoServidorHTTP = new TComunicacaoServidorHTTP(CHAVEDECOMUNICACAO);
  interfaceWireless = new TinterfaceWireless();

  interfaceWireless->conectaWifi();
  attachInterrupt(PINOPLUVIOMETRO, interrupcaoPluviometro, RISING);
  ultimaLeituraMenor = 0;
  ultimaLeituraMaior = 0;
}

int enviaDadoArquivo()
{

  boolean enviado = true;
  File dir = SPIFFS.open(DIRETORIODADOS);

  if (!dir)
  {
    return 0;
  }

  File arquivo = dir.openNextFile();

  while (arquivo)
  {
    String dadoStr;
    while (arquivo.available())
    {
      dadoStr = arquivo.readStringUntil('\n');
      if (!comunicacaoServidorHTTP->enviar(armazenamento->montaDadoString(dadoStr)))
      {
        enviado = false;
        break;
      }
    }

    if (enviado)
    {
      SPIFFS.remove(arquivo.name());
    }
    arquivo = dir.openNextFile();
  }
  dir.close();
  return 1;
}

void loop()
{

  if (time(NULL) - ultimaLeituraMenor > (INTERVALOENTRELEITURAS * 60) || ultimaLeituraMenor == 0)
  {

    TDado dado = sensorPluviometro->ler();
    if (dado.getValor() > 0 || time(NULL) - ultimaLeituraMenor > 3600 || ultimaLeituraMaior)
    {
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      armazenamento->armazenar(dado);
    }
    enviaDadoArquivo();

    ultimaLeituraMenor = time(NULL);
  }
}
