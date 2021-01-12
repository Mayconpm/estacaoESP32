#ifndef TUTILIDADESPARASTRINGS
#define TUTILIDADESPARASTRINGS
#include <Arduino.h>
#include <string.h>

class TUtilidadesParaStrings
{

public:
  static const String pegaEntreAspas(const String &texto, const int posicaoNoTexto)
  {
    int vc = 0;
    int inicio;
    int fim = -1;
    while (vc < posicaoNoTexto)
    {
      inicio = fim + 1;
      inicio = texto.indexOf('"', inicio) + 1;
      fim = texto.indexOf('"', inicio + 1);
      vc++;
    }
    if ((inicio == 0 && posicaoNoTexto > 1) || (fim == 0))
    {
      return "";
    }
    else
    {
      return texto.substring(inicio, fim);
    }
  }
};

#endif
