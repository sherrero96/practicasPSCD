//******************************************************************
// File    Saludador.cpp
// Author: Sergio Herrero Barco NIA: 698521
// Date:   Octubre 2016
// Coms:   Implementacion
//******************************************************************


#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Saludador.h"
using namespace std;

/*Constructor con parametros pasados por argumentos*/
Saludador::Saludador(string mens, int retardo, int veces, int iden) {
    this->mens = mens; //mens: parÃ¡metro de la funciÃ³n
                       //this->mens: variable propia
    this->retardo = retardo;
    this->veces = veces;
    this->iden = iden;
};

/*Constructor por defecto sin parametros*/
Saludador::Saludador() {
    mens = ""; //mens: parÃ¡metro de la funciÃ³n
                       //this->mens: variable propia
    retardo = 0;
    veces = 0;
    iden = 0;
};

/*Funcion que se limita  a escribir por pantalla el mensaje y dormirse*/
void Saludador::run() {
    for(int i=1; i<=veces; i++) {
        cout << mens + to_string(this->iden+1) + "\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
};
