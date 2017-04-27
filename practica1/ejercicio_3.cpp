//*****************************************************************
// File:   ejercicio_3.cpp
// Author: Sergio Herrero Barco NIA:698521
// Date:   Octubre 2016
// Coms:   Ejercicio 3 de la practica 1 de PSCD
//*****************************************************************


#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "Saludador.h"

const int MAX = 10;
using namespace std;


int main() {
    thread T[MAX];
    Saludador S[MAX];
    for(int i = 0; i < MAX; i++){
    string nombre(i,'\t'); //Constructor del string con las tabulaciones
    nombre = nombre + "Soy "; //
    /* Iniciamos cada hilo */
    S[i] = Saludador (nombre, (rand()%201 +100), (rand()%11 + 5), i);
    T[i] = thread(&Saludador::run, S[i]);
    }
    for(int i = 0; i< MAX; i++){
        T[i].join();
    }
    cout<<"Fin\n";
    return 0;
}
