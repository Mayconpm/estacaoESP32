#ifndef TUTILIDADESPARADATAS
#define TUTILIDADESPARADATAS
#include <Arduino.h>
#include <string.h>

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
    return passaDeTimestamp(now());
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

  static const int pegaHoraInt(const String &data)
  {
    return pegaHoraStr(data).toInt();
  }

  static const String pegaMinutoStr(const String &data)
  {
    return data.substring(14, 16);
  }

  static const int pegaMinutoInt(const String &data)
  {
    return pegaMinutoStr(data).toInt();
  }

  static const String pegaSegundoStr(const String &data)
  {
    return data.substring(17, 19);
  }

  static const int pegaSegundoInt(const String &data)
  {
    return pegaSegundoStr(data).toInt();
  }

  static const unsigned long int passaParaTimestamp(const String &data)
  {
    tmElements_t tm;
    tm.Year = pegaAnoInt(data) - 1970;
    tm.Month = pegaMesInt(data);
    tm.Day = pegaDiaInt(data);
    tm.Hour = pegaHoraInt(data);
    tm.Minute = pegaMinutoInt(data);
    tm.Second = pegaSegundoInt(data);

    time_t data2 = makeTime(tm);

    return (unsigned long int)data2;
  }

  static const String passaDeTimestamp(unsigned long int timestamp)
  {
    time_t data = timestamp;
    return passaParaTexto(year(data), month(data), day(data), hour(data), minute(data), second(data));
  }
};

#endif
