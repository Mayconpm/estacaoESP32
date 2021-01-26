#include <Arduino.h>
#include <esp_task_wdt.h>
#include "configuracoes.h"
#include "armazenamento.h"
#include "comunicacaoServidorHTTP.h"
#include "interfaceWireless.h"
#include "sensorPluviometro.h"
#include <esp_task_wdt.h>

TArmazenamento *armazenamento;
TComunicacaoServidorHTTP *comunicacaoServidorHTTP;
TinterfaceWireless *interfaceWireless;
TSensorPluviometro *sensorPluviometro;

void interrupcaoPluviometro()
{
  sensorPluviometro->interrupcao();
}
unsigned long int ultimaLeitura;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  armazenamento = new TArmazenamento();
  sensorPluviometro = new TSensorPluviometro();
  comunicacaoServidorHTTP = new TComunicacaoServidorHTTP(CHAVEDECOMUNICACAO);
  interfaceWireless = new TinterfaceWireless();

  esp_task_wdt_init(60, true);
  esp_task_wdt_add(NULL);

  attachInterrupt(PINOPLUVIOMETRO, interrupcaoPluviometro, RISING);
  ultimaLeitura = time(NULL);
  interfaceWireless->conectaWifi();
}

void loop()
{

  if (time(NULL) - ultimaLeitura > (INTERVALOENTRELEITURAS * 60))
  {
    TDado dado = sensorPluviometro->ler();
    interfaceWireless->conectaWifi();
    dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
    armazenamento->armazenar(dado);
    armazenamento->enviaDadoArquivo();
    ultimaLeitura = time(NULL);
  }
  esp_task_wdt_reset();
}
