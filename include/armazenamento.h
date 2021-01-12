#ifndef TARMAZENAMENTO
#define TARMAZENAMENTO
#include <Arduino.h>
#include <string.h>
#include "SPIFFS.h"
#include "comunicacaoServidorHTTP.h"
#include "dado.h"
#include "utilidadesParaStrings.h"
#include "utilidadesParaDatas.h"

TComunicacaoServidorHTTP *enviar(const TDado &dado);

class TArmazenamento
{

private:
  boolean inicializado;
  boolean enviado;
  File arquivo;

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
        // no more files
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

    String arquivo = DIRETORIODADOS;
    arquivo.concat(TUtilidadesParaDatas::pegaAnoStr(data) + "_");              // concatena o ano
    arquivo.concat(TUtilidadesParaDatas::pegaMesStr(data) + "_");              // concatena o mês
    arquivo.concat(TUtilidadesParaDatas::pegaDiaStr(data) + EXTENSAOARQUIVOS); // concatena o dia
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
    TDado dado(TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 1).toInt(), TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 2).toFloat(), TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 3));
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
  double pegaSomaMinutosAntecedentes(int tipo, unsigned long int minutos)
  {
    double soma = 0;
    unsigned long int momentoFinal = time(NULL);
    // unsigned long int momentoFinal = now();
    unsigned long int momentoInicial = momentoFinal - (minutos * 60);

    unsigned long int diasDiferenca = ceil((momentoFinal - momentoInicial) / 86400.0);

    for (int i = 0; i < diasDiferenca; i++)
    {
      soma += this->somaDadosDiretorio(tipo, this->montaArquivoTemporario(TUtilidadesParaDatas::passaDeTimestamp(momentoFinal - (i * 86400))), momentoInicial, momentoFinal);
    }
    return soma;
  }

  double pegaSomaHorasAntecedentes(int tipo, int horas)
  {
    return this->pegaSomaMinutosAntecedentes(tipo, horas * 60);
  }

  double pegaSomaDiasAntecedentes(int tipo, int dias)
  {
    return this->pegaSomaHorasAntecedentes(tipo, dias * 24);
  }

  double pegaSomaMesesAntecedentes(int tipo, int meses)
  {
    return this->pegaSomaDiasAntecedentes(tipo, meses * 30);
  }

  void apagaArquivo()
  {
    this->arquivo.close();
    File dir = this->open(DIRETORIODADOS);

    if (!dir)
    {
      return;
    }

    String dadoStr = "";
    while (true)
    {
      this->arquivo = dir.openNextFile();
      SPIFFS.remove(arquivo.name());
    }
    dir.close();
  }

  boolean armazenamentoInicializado()
  {
    return this->inicializado;
  }

  ~TArmazenamento() {}
};

#endif
