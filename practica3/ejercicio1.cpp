//*****************************************************************
// File:    ejercicio1.cpp
// Author:  Sergio Herrero NIA:698521
// Date:    Noviembre 2016
// Coms:    Ejercicio 1 de la practica 3
//*****************************************************************

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "Semaphore.h"


using namespace std;

const int MAX = 5; //Las veces que va a recorrer
const int MAXCOCHES = 20; //Cantidad de coches que van a circular


Semaphore surtidores(4); //Semaforo que representa los cuatro surtidores
Semaphore ocupado(1); //Semaforo para que solo un proceso busque que surtidor esta disponible
bool estaVacio[4]; //Indica cual de todos los surtidores estan vacios

/* Pre: Al menos una componente del vector 'estaVacio', es 'true'
 * Post: Devuelve la componente del vector que es vacio
 */ 
int libre(){
	for(int i = 0; i < 4; i++){
		if(estaVacio[i] == true) return i;
	}
}

/* Proceso que recorre durante un tiempo, y cuando se queda sin gasolina va a repostar. Una vez que accede
 * a un surtidor vacio, reposta, y cuando esta completo vueve a circular.
 */
void vehiculo(int j){
	int surti;
	for(int i = 0; i < MAX; i++){
		//Recorre unos kilometros
		std::this_thread::sleep_for(std::chrono::seconds(rand()%3));
		//Se queda sin gasolina, accede a un surtidor
		surtidores.wait();
		/*EXCLUSIÓN MUTUA*/
		ocupado.wait();
			surti = libre(); //Identificador de que surtidor esta libre
			estaVacio[surti] = false;		
			printf("Surtidor %d: COCHE %d\n",surti, j);
		ocupado.signal();
		/*FIN EXCLUSIÓN MUTUA*/
		/*Repostamos durante 0.5 segundos*/
		this_thread::sleep_for(chrono::milliseconds(500));
		printf("Surtidor %d: Libre*****\n",surti);
		estaVacio[surti] = true;
		surtidores.signal();
	}
}

/* Inicia el vector estaVacio con todo valores 'true' */
void iniciarVector(){
	for(int i = 0; i < 4; i++)
		estaVacio[i] = true;
}

int main(){
	iniciarVector(); //Iniciamos los surtidores; todos estan libres
	thread V[MAXCOCHES];
	for(int i = 0; i < MAXCOCHES; i++){
		V[i] = thread(&vehiculo, i);
	}
	for(int i = 0; i < MAXCOCHES; i++){
		V[i].join();
	}

}

