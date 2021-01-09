#include <Arduino.h>
#include <TimeLib.h>
#include "configuracoes.h"
#include "sensorPluviometro.h"

TSensorPluviometro *sensorPluviometro;

void interrupcaoPluviometro()
{
  sensorPluviometro->interrupcao();
}

unsigned long int ultimaLeitura;

void setup()
{
  Serial.begin(115200);
  sensorPluviometro = new TSensorPluviometro();

  attachInterrupt(digitalPinToInterrupt(sensorPluviometro->getPino()), interrupcaoPluviometro, RISING);
}

void loop()
{

  if (now() - ultimaLeitura > (INTERVALOENTRELEITURAS * 60) || ultimaLeitura == 0)
  {
    TDado dado = sensorPluviometro->ler();
    ultimaLeitura = now();
  }
}
