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

public:
  //Construtor:
  TDado(const int &tipo = 0, const double &valor = 0, const String &data = "")
  {
    this->tipo = tipo;
    this->valor = valor;
    this->data = data;
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
