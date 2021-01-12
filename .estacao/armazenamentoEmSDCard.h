#ifndef TARMAZENAMENTOEMSDCARD
#define TARMAZENAMENTOEMSDCARD
#include <Arduino.h>
#include <string.h>
#include "armazenamento.h"
#include "utilidadesParaStrings.h"
#include "utilidadesParaDatas.h"

class TArmazenamentoEmSDCard : public TArmazenamento
{

private:
  boolean inicializado;
  File arquivo;

  File open(const String &diretorio, uint8_t modo = FILE_READ)
  {
    char diretorioChar[diretorio.length() + 1];
    diretorio.toCharArray(diretorioChar, sizeof(diretorioChar));
    return SD.open(diretorioChar, modo);
  }

  boolean exists(const String &diretorio)
  {
    char diretorioChar[diretorio.length() + 1];
    diretorio.toCharArray(diretorioChar, sizeof(diretorioChar));
    return SD.exists(diretorioChar);
  }

  boolean mkdir(const String &diretorio)
  {
    char diretorioChar[diretorio.length() + 1];
    diretorio.toCharArray(diretorioChar, sizeof(diretorioChar));
    return SD.mkdir(diretorioChar);
  }

  boolean remove(const String &diretorio)
  {
    char diretorioChar[diretorio.length() + 1];
    diretorio.toCharArray(diretorioChar, sizeof(diretorioChar));
    return SD.remove(diretorioChar);
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

    //caso o diretório não exista, retorna 0
    if (!this->exists(diretorio))
    {
      return 0;
    }

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
      if (!this->arquivo.isDirectory() && this->arquivo)
      {
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
      }
      this->arquivo.close();
    }
    dir.close();
    return soma;
  }

  const boolean abreArquivo(const String &caminho, const boolean &escrita = false)
  {
    this->arquivo.close(); // fecha um possível arquivo aberto
    String caminhoNovo = "";

    if (caminho[0] != '/')
    { //se não começar com barra
      caminhoNovo = "/";
    }
    caminhoNovo.concat(caminho); //adiciona a barra

    if (caminhoNovo.lastIndexOf('/') > 0 && escrita)
    {                                                                                //se o arquivo não estiver na raiz do cartão
      String diretorio = caminhoNovo.substring(0, caminhoNovo.lastIndexOf('/') + 1); //pega o diretório sem o nome do arquivo

      //caso o diretório do arquivo não exista, ele será criado
      if (!this->exists(diretorio))
      {
        this->mkdir(diretorio);
      }
    }

    this->arquivo = escrita ? this->open(caminhoNovo, FILE_WRITE) : this->open(caminhoNovo);
    return (boolean)this->arquivo;
  }

  const String montaDiretorio(const String &data)
  {
    String diretorio = DIRETORIODADOS;
    diretorio.concat(TUtilidadesParaDatas::pegaAnoStr(data) + "_"); // concatena o ano
    diretorio.concat(TUtilidadesParaDatas::pegaMesStr(data) + "_"); // concatena o mês
    diretorio.concat(TUtilidadesParaDatas::pegaDiaStr(data) + "_"); // concatena o dia
    return diretorio;
  }

  const String montaCaminho(const TDado &dado)
  {
    String caminho = this->montaDiretorio(dado.getData());
    caminho.concat(TUtilidadesParaDatas::pegaHoraStr(dado.getData()) + EXTENSAOARQUIVOS); // concatena a hora e a extensão do arquivo
    return caminho;
  }

  const String montaCaminhoArquivoTemporario(const TDado &dado)
  {
    String caminho = "";
    do
    {
      caminho = DIRETORIOARQUIVOSTEMPORARIOS;
      caminho.concat(dado.getTipo());              // concatena o tipo
      caminho.concat(random(1000000, 9999999));    // concatena um valor aleatório
      caminho.concat(EXTENSAOARQUIVOSTEMPORARIOS); // concatena a hora e a extensão do arquivo
    } while (this->exists(caminho));
    return caminho;
  }

  const TDado montaDadoString(const String &dadoStr)
  {
    TDado dado(TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 1).toInt(), TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 2).toFloat(), TUtilidadesParaStrings::pegaEntreAspas(dadoStr, 3));
    return dado;
  }

public:
  TArmazenamentoEmSDCard()
  {
    this->inicializado = false;
    TLog::envia("INICIALIZANDO SD CARD...");
    int tentativa = 1;
    while (tentativa <= TENTATIVASSDCARD)
    {
      String temp = "TENTANDO INICIALIZAR SD CARD...... [";
      temp.concat(tentativa);
      temp.concat("]");
      TLog::envia(temp.c_str());
      if (SD.begin(4))
      {
        this->inicializado = true;
        TLog::envia("SD CARD INICIALIZADO");
        break;
      }
      tentativa++;
    }
  }

  boolean armazenar(const TDado &dado)
  {
    boolean resposta = false;
    if (this->abreArquivo(montaCaminho(dado), true))
    {
      this->arquivo.println(dado.toString());
      resposta = true;
    }
    this->arquivo.close();
    if (!dado.getTransmitido() && this->abreArquivo(montaCaminhoArquivoTemporario(dado), true))
    { //se o dado não tiver sido transmitido e caso consiga abrir o arquivo
      this->arquivo.println(dado.toString());
    }
    this->arquivo.close();
    return resposta;
  }

  double pegaSomaMinutosAntecedentes(int tipo, unsigned long int minutos)
  {
    double soma = 0;
    unsigned long int momentoFinal = now();
    unsigned long int momentoInicial = momentoFinal - (minutos * 60);

    unsigned long int diasDiferenca = ceil((momentoFinal - momentoInicial) / 86400.0);

    for (int i = 0; i < diasDiferenca; i++)
    {
      soma += this->somaDadosDiretorio(tipo, this->montaDiretorio(TUtilidadesParaDatas::passaDeTimestamp(momentoFinal - (i * 86400))), momentoInicial, momentoFinal);
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

  TDado pegaDadoTemporario()
  {
    //caso o diretório não exista, retorna 0
    if (!this->exists(DIRETORIOARQUIVOSTEMPORARIOS))
    {
      return 0;
    }

    this->arquivo.close();
    File dir = this->open(DIRETORIOARQUIVOSTEMPORARIOS);

    if (!dir)
    {
      return 0;
    }

    String dadoStr;
    int vc = 0;
    while (true && vc < 10)
    {
      this->arquivo = dir.openNextFile();
      if (this->arquivo && !this->arquivo.isDirectory())
      {
        String dadoStr;
        while (this->arquivo.available())
        {
          dadoStr = arquivo.readStringUntil('\n');
          Serial.println(dadoStr);
        }
        break;
      }
      this->arquivo.close();
      vc++;
    }
    dir.close();
    return this->montaDadoString(dadoStr);
  }

  void apagaDadoTemporario()
  {
    //caso o diretório não exista, retorna 0
    if (!this->exists(DIRETORIOARQUIVOSTEMPORARIOS))
    {
      return;
    }

    this->arquivo.close();
    File dir = this->open(DIRETORIOARQUIVOSTEMPORARIOS);

    if (!dir)
    {
      return;
    }

    String dadoStr = "";
    while (true)
    {
      this->arquivo = dir.openNextFile();
      if (this->arquivo && !this->arquivo.isDirectory())
      {
        String temp = DIRETORIOARQUIVOSTEMPORARIOS;
        temp.concat(this->arquivo.name());
        this->remove(temp);
        break;
      }
      this->arquivo.close();
    }
    dir.close();
  }

  ~TArmazenamentoEmSDCard() {}
};

#endif
