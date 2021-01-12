#ifndef TDADO
#define TDADO
#include <Arduino.h>
#include <string.h>

class TDado
{

private:
  int tipo;
  double valor;
  String data;
  boolean transmitido;

public:
  //Construtor:
  TDado(const int &tipo = 0, const double &valor = 0, const String &data = "1970-01-01_00:00:00", const boolean &transmitido = false)
  {
    this->tipo = tipo;
    this->valor = valor;
    this->data = data;
    this->transmitido = transmitido;
  }

  //Getters e setters do tipo:
  const int &getTipo() const
  {
    return this->tipo;
  }

  void setTipo(const int &tipo)
  {
    this->tipo = tipo;
  }

  //Getters e setters do valor:
  const double &getValor() const
  {
    return this->valor;
  }

  void setValor(const double &valor)
  {
    this->valor = valor;
  }

  //Getters e setters da data:
  const String &getData() const
  {
    return this->data;
  }

  void setData(const String &data)
  {
    this->data = data;
  }

  //Getters e setters de transmitido:
  const boolean &getTransmitido() const
  {
    return this->transmitido;
  }

  void setTransmitido(const double &transmitido)
  {
    this->transmitido = transmitido;
  }

  const String toString() const
  { //segundo const para indicar que o método não altera o objeto
    return "\"" + String(this->tipo) + "\", \"" + String(this->valor) + "\", \"" + this->data + "\"";
  }

  //Destrutor:
  ~TDado()
  {
  }
};

#endif
