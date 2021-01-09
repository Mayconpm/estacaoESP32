#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>

#include "configuracoes.h"


#include "dado.h"
#include "armazenamentoEmSDCard.h"
#include "sensorPluviometro.h"

#include <MemoryFree.h>

TArmazenamentoEmSDCard * armazenamento;
TSensorPluviometro * sensorPluviometro;

void interrupcaoPluviometro() {
    sensorPluviometro->interrupcao();
}

// alarme
boolean alarmeAtivo;
unsigned long int ultimoAlarme;

unsigned long int ultimaLeitura;

void setup() {
  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) {
     Serial.println("Erro RTC");
  } else {
    Serial.println(TUtilidadesParaDatas::pegaDataAtualEmTexto());
  }
  
  armazenamento = new TArmazenamentoEmSDCard();
  
  sensorPluviometro = new TSensorPluviometro();
  
  attachInterrupt(digitalPinToInterrupt(sensorPluviometro->getPino()),interrupcaoPluviometro,RISING);

  // alarme
  pinMode(PINOSIRENE,OUTPUT);
  alarmeAtivo = false;
  ultimoAlarme = 0;
  
  ultimaLeitura = 0;
}

void loop() {
  if(now() - ultimaLeitura > (INTERVALOENTRELEITURAS * 60) || ultimaLeitura == 0) {
    TDado dado = sensorPluviometro->ler();
    armazenamento->armazenar(dado);
    ultimaLeitura = now();
    Serial.println(freeMemory());
  }

  //alarme
  if(armazenamento->pegaSomaMinutosAntecedentes(1, 5)*20>=30){
    alarmeAtivo = true;
  }

  if(armazenamento->pegaSomaMinutosAntecedentes(1, 1380) >= 70 && armazenamento->pegaSomaMinutosAntecedentes(1, 5)*20>=25){ //23 horas
    alarmeAtivo = true;
  }

  if(armazenamento->pegaSomaMinutosAntecedentes(1, 4320)>=45 && armazenamento->pegaSomaMinutosAntecedentes(1, 5)*(20)>=20){ //72 horas
    alarmeAtivo = true;
  }

  if(armazenamento->pegaSomaMinutosAntecedentes(1, 43200)>=250){ //30 dias
    alarmeAtivo = true;
  }

  if(armazenamento->pegaSomaMinutosAntecedentes(1, 5)>=KORSI / pow(armazenamento->pegaSomaMinutosAntecedentes(1, 5760),0.933)){ //96 horas
    alarmeAtivo = true;
  }

  if(armazenamento->pegaSomaMinutosAntecedentes(1, 360)==0){ //6 horas
    alarmeAtivo = false;
  }

  if (alarmeAtivo) {
    digitalWrite(PINOSIRENE, HIGH);
    unsigned long int atual = now();
    while(now() - atual < DURACAOPULSO) {}
    digitalWrite(PINOSIRENE, LOW);
  }
}
