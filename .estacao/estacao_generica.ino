#include "configuracoes.h"

//#include "configuracoesIPRJ.h"
//#include "configuracoesMacae.h"
//#include "configuracoesTeste.h"
//#include "configuracoesCaledonia.h"
#include "configuracoesTeste.h"

#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include "log.h"
#include "dado.h"
#include "armazenamentoEmSDCard.h"
#include <MemoryFree.h>

#ifdef WATCHDOG
  #include <avr/wdt.h>
  #include <TimerOne.h>
  #include "watchdog.h"
#endif

//void(* resetarArduino) (void) = 0;//função para reset do Arduino
void resetarArduino() {
  while(1);
}

#ifdef RESETDIARIO
  int diaInicializacao;
#endif

//Comunicação

#ifdef ETHERNET
  #include <Ethernet.h>
  #include "comunicacaoServidorHTTP.h"
  TComunicacaoServidorHTTP * comunicacaoServidorHTTP;
#endif

#ifdef XBEE
  #include <MD5.h>
  #include "comunicacaoXbee.h"
  TComunicacaoXbee * comunicacaoXbee;
#endif


//Sensores

#ifdef PLUVIOMETRO
  #include "sensorPluviometro.h"
  TSensor * sensorPluviometro;
  
  void interrupcaoPluviometro() {
    sensorPluviometro->interrupcao();
  }
#endif





TArmazenamento * armazenamento;


unsigned long int ultimaLeituraMaior;
unsigned long int ultimaLeituraMenor;


#ifdef WATCHDOG
  TWatchdog * watchdog;

  void interrupcaoWatchdog() {
    watchdog->interrupcao();
  }
#endif

void setup() {

  #ifdef WATCHDOG
    watchdog = new TWatchdog(TEMPOWATCHDOG);
    Timer1.initialize(1000000);
    Timer1.attachInterrupt( interrupcaoWatchdog );
  #endif
  
  Serial.begin(9600);
  
  String temp = "Compilado em: ";
  temp.concat(__DATE__);
  temp.concat(" ");
  temp.concat(__TIME__);
  TLog::envia(temp.c_str());
  
  setSyncProvider(RTC.get);
  if(timeStatus()!= timeSet) {
    TLog::envia("Problema ao definir o RTC como relogio do sistema");
    delay(100);
    resetarArduino();
  } else {
    TLog::envia("RTC definido como relogio do sistema com sucesso");
  }

  #ifdef WATCHDOG
    watchdog->reset();
  #endif
  
  armazenamento = new TArmazenamentoEmSDCard();
  if (!armazenamento->armazenamentoInicializado()) {
   delay(100);
   resetarArduino();
  }

  #ifdef WATCHDOG
    watchdog->reset();
  #endif



  #ifdef XBEE
  comunicacaoXbee = new TComunicacaoXbee(CHAVEDECOMUNICACAO);
  #endif

  #ifdef WATCHDOG
    watchdog->reset();
  #endif

  //Sensores

  #ifdef PLUVIOMETRO
    sensorPluviometro = new TSensorPluviometro();
    attachInterrupt(digitalPinToInterrupt(sensorPluviometro->getPino()),interrupcaoPluviometro,RISING);
  #endif
  
  #ifdef ANEMOMETRO
    sensorAnemometro = new TSensorAnemometro();
    attachInterrupt(digitalPinToInterrupt(sensorAnemometro->getPino()),interrupcaoAnemometro,RISING);
  #endif
  
  #ifdef TERMOMETROEHIGROMETRO
    sensorTemperatura = new TSensorTemperatura();
    sensorUmidade = new TSensorUmidade();
    dht.begin();
    delay(2000);
  #endif
  
  #ifdef PIRANOMETRO
    sensorPiranometro = new TSensorPiranometro();
  #endif
  
  #ifdef BAROMETRO
    sensorBarometro = new TSensorBarometro();
  #endif

  #ifdef TERMOMETROVIABAROMETRO
    sensorTemperaturaViaBarometro = new TSensorTemperaturaViaBarometro();
  #endif

  
  #ifdef ALARME
    pinMode(PINOSIRENE,OUTPUT);
    alarmeAtivo = false;
    ultimoAlarme = 0;
  #endif

  #ifdef RESETDIARIO
    diaInicializacao = TUtilidadesParaDatas::pegaDiaInt(TUtilidadesParaDatas::pegaDataAtualEmTexto());
  #endif
  
  ultimaLeituraMaior = 0;
  ultimaLeituraMenor = 0;

  #ifdef ALARME
    ultimaChecagemAlarme = 0;
  #endif

  #ifdef WATCHDOG
    watchdog->reset();
  #endif
  
  delay(1000);
  
}



void loop() {

  #ifdef WATCHDOG
    watchdog->reset();
  #endif
  if(now() - ultimaLeituraMaior > INTERVALOMAIORENTRELEITURASMAIOR || ultimaLeituraMaior == 0) {
    TDado dado;

    #ifdef PLUVIOMETRO
      dado = sensorPluviometro->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    #ifdef WATCHDOG
      watchdog->reset();
    #endif

    #ifdef TERMOMETROEHIGROMETRO
      dado = sensorTemperatura->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
      
      dado = sensorUmidade->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    #ifdef WATCHDOG
      watchdog->reset();
    #endif

    #ifdef BAROMETRO
      dado = sensorBarometro->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    #ifdef WATCHDOG
      watchdog->reset();
    #endif

    #ifdef TERMOMETROVIABAROMETRO
      dado = sensorTemperaturaViaBarometro->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    ultimaLeituraMaior = now();
    
    String temp = "memoria livre(intervalo maior):";
    temp.concat(freeMemory());
    TLog::envia(temp.c_str());
  }

  #ifdef WATCHDOG
    watchdog->reset();
  #endif

  if(now() - ultimaLeituraMenor > INTERVALOMENORENTRELEITURASMENOR || ultimaLeituraMenor == 0) {
    TDado dado;

    #ifdef ANEMOMETRO
      dado = sensorAnemometro->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    #ifdef WATCHDOG
      watchdog->reset();
    #endif

    #ifdef PIRANOMETRO
      dado = sensorPiranometro->ler();
      #ifdef ETHERNET
      dado.setTransmitido(comunicacaoServidorHTTP->enviar(dado));
      #endif
      armazenamento->armazenar(dado);
    #endif

    ultimaLeituraMenor = now();
    
    String temp = "memoria livre(intervalo menor):";
    temp.concat(freeMemory());
    TLog::envia(temp.c_str());
  }

  #ifdef WATCHDOG
    watchdog->reset();
  #endif

  //Envio dos dados
  
  #ifdef ETHERNET
  if (armazenamento->existeDadoTemporario()) {
    TDado dadoTemp = armazenamento->pegaDadoTemporario();
    if (comunicacaoServidorHTTP->enviar(dadoTemp)) {
      armazenamento->apagaDadoTemporario();
    }
  }
  #endif

  #ifdef XBEE
  if (comunicacaoXbee->sucessoUltimoEnvio() && armazenamento->existeDadoTemporario()) {
    armazenamento->apagaDadoTemporario();
  }

  if (armazenamento->existeDadoTemporario()) {
    TDado dadoTemp = armazenamento->pegaDadoTemporario();
    comunicacaoXbee->enviar(dadoTemp);
  }
  #endif

  #ifdef WATCHDOG
    watchdog->reset();
  #endif

  #ifdef ALARME
  if(now() - ultimaChecagemAlarme > (INTERVALOCHECAGENSALARME * 60) || ultimaChecagemAlarme == 0) {
      TLog::envia("Atualizando alarme");
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 5)*20>=30){
        alarmeAtivo = true;
      }

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
    
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 1380) >= 70 && armazenamento->pegaSomaMinutosAntecedentes(1, 5)*20>=25){ //23 horas
        alarmeAtivo = true;
      }

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
    
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 4320)>=45 && armazenamento->pegaSomaMinutosAntecedentes(1, 5)*(20)>=20){ //72 horas
        alarmeAtivo = true;
      }

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
      
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 43200)>=250){ //30 dias
        alarmeAtivo = true;
      }
      
      #ifdef WATCHDOG
        watchdog->reset();
      #endif
    
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 5)>=KORSI / pow(armazenamento->pegaSomaMinutosAntecedentes(1, 5760),0.933)){ //96 horas
        alarmeAtivo = true;
      }

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
    
      if(armazenamento->pegaSomaMinutosAntecedentes(1, 360)==0){ //6 horas
        alarmeAtivo = false;
      }

      #ifdef WATCHDOG
        watchdog->reset();
      #endif
      
      ultimaChecagemAlarme = now();
      TLog::envia("Final da atualizacao do alarme");
  }
  
    if (alarmeAtivo) {
      digitalWrite(PINOSIRENE, HIGH);
      unsigned long int atual = now();
      while(now() - atual < DURACAOPULSO) {}
      digitalWrite(PINOSIRENE, LOW);
    }
  #endif

  #ifdef RESETDIARIO
    if (diaInicializacao != TUtilidadesParaDatas::pegaDiaInt(TUtilidadesParaDatas::pegaDataAtualEmTexto())) {
      delay(100);
      resetarArduino();
    }
  #endif

  

}
