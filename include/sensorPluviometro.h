#ifndef TSENSORPLUVIOMETRO
#define TSENSORPLUVIOMETRO
#include <Arduino.h>
#include <string.h>
#include "dado.h"
#include "utilidadesParaDatas.h"

class TSensorPluviometro
{

private:
  int tipo;
  int pino;
  int numeroDeBasculadas;
  unsigned long int ultimaBasculada;

public:
  TSensorPluviometro(int tipo = 1, int pino = 18)
  {
    this->tipo = tipo;
    this->pino = pino;
    this->ultimaBasculada = 0;
    pinMode(this->pino, INPUT_PULLUP);
    this->numeroDeBasculadas = 0;
  }

  TDado ler()
  {
    Serial.println("Pluviometro lido");
    float valor = 1.0 * this->numeroDeBasculadas * VOLUMEBASCULA;
    this->numeroDeBasculadas = 0;
    TDado dado(this->tipo, valor, TUtilidadesParaDatas::pegaDataAtualEmTexto());
    return dado;
  }

  void IRAM_ATTR interrupcao()
  {
    if (((millis() - this->ultimaBasculada) >= TEMPOMINIMOENTREBASCULADAS) || millis() < this->ultimaBasculada)
    {
      Serial.println("Basculada capturada");
      this->numeroDeBasculadas++;
      this->ultimaBasculada = millis();
    }
  }

  int getPino()
  {
    return this->pino;
  }
};

#endif
