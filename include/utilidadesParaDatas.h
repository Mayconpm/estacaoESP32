#ifndef TUTILIDADESPARADATAS
#define TUTILIDADESPARADATAS
#include <Arduino.h>
#include <string.h>
#include "time.h"
#include "TimeLib.h"

class TUtilidadesParaDatas
{

private:
  static const String adicionaZero(const int &numero)
  {
    String tmp = "";
    if (numero < 10)
    {
      tmp = "0";
    }
    tmp.concat(String(numero));
    return tmp;
  }

public:
  static const String passaParaTexto(const int &ano, const int &mes, const int &dia, const int &hora, const int &minuto, const int &segundo)
  {
    String tmp = "";
    tmp.concat(ano);
    tmp.concat('-');
    tmp.concat(adicionaZero(mes));
    tmp.concat('-');
    tmp.concat(adicionaZero(dia));
    tmp.concat('_');
    tmp.concat(adicionaZero(hora));
    tmp.concat(':');
    tmp.concat(adicionaZero(minuto));
    tmp.concat(':');
    tmp.concat(adicionaZero(segundo));
    return tmp;
  }

  static const String pegaDataAtualEmTexto()
  {
    return passaDeTimestamp(time(NULL));
  }

  static const String pegaAnoStr(const String &data)
  {

    return data.substring(0, 4);
  }

  static const int pegaAnoInt(const String &data)
  {
    return pegaAnoStr(data).toInt();
  }

  static const String pegaMesStr(const String &data)
  {
    return data.substring(5, 7);
  }

  static const int pegaMesInt(const String &data)
  {
    return pegaMesStr(data).toInt();
  }

  static const String pegaDiaStr(const String &data)
  {
    return data.substring(8, 10);
  }

  static const int pegaDiaInt(const String &data)
  {
    return pegaDiaStr(data).toInt();
  }

  static const String pegaHoraStr(const String &data)
  {
    return data.substring(11, 13);
  }

  static const unsigned long int passaParaTimestamp(const String &data)
  {
    struct tm datatm;
    strptime(data.c_str(), "%F_%H:%M:%S", &datatm);
    time_t data2 = mktime(&datatm);
    return (unsigned long int)data2;
  }

  static const String passaDeTimestamp(unsigned long int timestamp)
  {
    time_t t = timestamp;
    return passaParaTexto(year(t), month(t), day(t), hour(t) - 3, minute(t), minute(t));
  }
};

#endif
