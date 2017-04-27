//*****************************************************************
// File:   ejercicio_2.cpp
// Author: Sergio Herrero Barco NIA:698521
// Date:   Octubre 2016
// Coms:   Ejercicio 2 de la practica 1 de PSCD
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

const int MAX = 10;	//Procesos que vamos a ejecutar
using namespace std;

/*
 * Pre: veces > 0 ^ retardo > 0
 * Post: Muestra por pantalla el string m, [veces] veces
 */
void saludo(string m, int retardo, int veces) {
    for(int i=1; i<=veces; i++) {
        cout << m+"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}


int main() {
    thread P[MAX]; //Vector de threads
    srand(time(NULL)); //Semilla para numeros pseudo-aleatorios
    for(int i = 0; i < MAX; i++){
     string nombre(i,'\t'); //Constructor del string con las tabulaciones
     nombre = nombre + "Soy " + to_string(i+1);
        /* Iniciamos cada hilo */
        P[i] = thread(&saludo, nombre,(rand()%201 +100), (rand()%11 + 5));
    }

    /* Lanzamos la ejecucion de los hilos */
    for(int i = 0; i<MAX; i++){
        P[i].join();
    }

    cout << "Fin\n";
    return 0;
}
