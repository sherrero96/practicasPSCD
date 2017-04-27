//*********************************************************
// File    Saludador.h
// Author: Sergio Herrero Barco NIA: 698521
// Date:   Octubre 2016
// Coms:   Parte de Especificacion: variables y funciones
//         de los objetos de esta clase
//*********************************************************

#ifndef SALUDADOR_H
#define SALUDADOR_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

class Saludador {
public:
    Saludador(string mens, int retardo, int veces, int iden);
    Saludador(); //constructor por defecto sin argumentos
    void run();
private:
	/*Los campos de la clase Saludador*/
    string mens; //El mensaje
    int retardo, veces, iden;
};

#endif
