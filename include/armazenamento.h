#ifndef TARMAZENAMENTO
#define TARMAZENAMENTO
#include <Arduino.h>
#include <string.h>
#include "SPIFFS.h"
#include "comunicacaoServidorHTTP.h"
#include "dado.h"
#include "utilidadesParaStrings.h"
#include "utilidadesParaDatas.h"

class TArmazenamento
{
private:
  boolean inicializado;
  boolean enviado;
  File arquivo;
  TComunicacaoServidorHTTP *comunicacaoServidorHTTP;

  File open(const String &arquivo, const char *modo = FILE_READ)
  {
    return SPIFFS.open(arquivo, modo);
  }

  void removeFile(const String &arquivo)
  {
    SPIFFS.remove(arquivo);
  }

  const float pegaValorLinha(int tipo, const String &linha, const unsigned long int &momentoInicial, const unsigned long int &momentoFinal)
  {
    if (TUtilidadesParaStrings::pegaEntreAspas(linha, 1).toInt() != tipo)
    { //se não for do tipo desejado
      return 0;
    }
    unsigned long int data = TUtilidadesParaDatas::passaParaTimestamp(TUtilidadesParaStrings::pegaEntreAspas(linha, 3));
    if (data >= momentoInicial && data <= momentoFinal)
    {
      return TUtilidadesParaStrings::pegaEntreAspas(linha, 2).toFloat();
    }
    return 0;
  }

  const double somaDadosDiretorio(int tipo, const String &diretorio, const unsigned long int &momentoInicial, const unsigned long int &momentoFinal)
  {
    double soma = 0;

    this->arquivo.close();
    File dir = this->open(diretorio);

    if (!dir)
    {
      return 0;
    }

    while (true)
    {
      this->arquivo = dir.openNextFile();
      if (!this->arquivo)
      {
        break;
      }
      String bufferStr = "";
      while (this->arquivo.available())
      {
        char lido = this->arquivo.read();
        if (lido != 10)
        { //10 = quebra de linha
          bufferStr.concat(lido);
        }
        else
        {
          soma += pegaValorLinha(tipo, bufferStr, momentoInicial, momentoFinal);
          bufferStr = "";
        }
      }
      soma += pegaValorLinha(tipo, bufferStr, momentoInicial, momentoFinal);

      this->arquivo.close();
    }
    dir.close();
    return soma;
  }

  const boolean abreArquivo(const String &arquivo, const boolean &escrita = false)
  {
    this->arquivo.close(); // fecha um possível arquivo aberto

    this->arquivo = escrita ? this->open(arquivo, "a") : this->open(arquivo);

    return (boolean)this->arquivo;
  }

  const String montaArquivoTemporario(const String &data)
  {
    String arquivo = "/";
    arquivo.concat(TUtilidadesParaDatas::pegaAnoStr(data) + "_");    // concatena o ano
    arquivo.concat(TUtilidadesParaDatas::pegaMesStr(data) + "_");    // concatena o mês
    arquivo.concat(TUtilidadesParaDatas::pegaDiaStr(data) + ".txt"); // concatena o dia
    return arquivo;
  }

public:
  TArmazenamento()
  {
    this->inicializado = false;
    if (SPIFFS.begin())
    {
      this->inicializado = true;
      Serial.println("Sistema de arquivos montado com sucesso.");
    }
    else
    {
      this->inicializado = false;
      Serial.println("Erro ao montar o sitema de arquivos.");
    }
  };

  const TDado montaDadoString(const String &dadoStr)
  {
    TDado dado(TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 1).toFloat(), TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 2));
    return dado;
  }

  boolean armazenar(const TDado &dado)
  {
    boolean resposta = false;
    if (!dado.getTransmitido() && this->abreArquivo(montaArquivoTemporario(dado.getData()), true))
    { //se o dado não tiver sido transmitido e caso consiga abrir o arquivo
      this->arquivo.println(dado.toString());
      resposta = true;
      Serial.println("Dados salvos no arquivo.");
    }
    this->arquivo.close();

    return resposta;
  }

  int enviaDadoArquivo()
  {
    boolean enviado = true;
    File dir = open("/");
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
        if (comunicacaoServidorHTTP->enviar(montaDadoString(dadoStr)))
        {
          enviado = false;
          break;
        }
      }
      if (enviado)
      {
        Serial.println("remover arquivo");
        remove(arquivo.name());
      }
      arquivo = dir.openNextFile();
    }
    dir.close();
    return 1;
  }
  ~TArmazenamento() {}
};

#endif
