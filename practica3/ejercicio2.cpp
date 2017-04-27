//*****************************************************************
// File:    ejercicio2.cpp
// Author:  Sergio Herrero NIA:698521
// Date:    Noviembre 2016
// Coms:    Ejercicio 2 de la practica 3
//*****************************************************************

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "Semaphore.h"


using namespace std;

const int MAX = 5; //Las veces que va a reiterar el algoritmo
const int MAXCOCHES = 20; //Cantidad de coches que van a circular
const int MAXREVISOR = 2; //Cantidad de veces que el revisor arreglara


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
	srand(time(NULL));
	for(int i = 0; i < MAX; i++){
		//Recorre unos kilometros		
		std::this_thread::sleep_for(std::chrono::seconds(rand()%4));
		//Se queda sin gasolina, accede a un surtidor
		surtidores.wait();
		/*EXCLUSIÓN MUTUA*/
		ocupado.wait();
			surti = libre(); //Identificador de que surtidor esta libre
			estaVacio[surti] = false;		
			printf("Surtidor %d: [COCHE %d]\n",surti, j);
		ocupado.signal();
		/*FIN EXCLUSIÓN MUTUA*/
		/*Repostamos durante 0.5 segundos*/
		this_thread::sleep_for(chrono::milliseconds(500));
		printf("Surtidor %d: Libre*****\n",surti);
		estaVacio[surti] = true;
		surtidores.signal();
	}
}

/* Proceso que recorre durante un tiempo, y cuando se queda sin gasolina va a repostar. Una vez que accede
 * a un surtidor vacio, reposta, y cuando esta completo vueve a circular.
 */
void revisor(){
	int surti;
	for(int i = 0; i < MAXREVISOR; i++){
		this_thread::sleep_for(chrono::milliseconds(1500));
		//Esperamos a que esten los cuatro libres		
		for(int i = 0; i < 4; i++){
			surtidores.wait();
		}
		/*EXCLUSIÓN MUTUA*/
		ocupado.wait();
			printf("Surtidor %d: Arreglando*****\n",0);
			printf("Surtidor %d: Arreglando*****\n",1);
			printf("Surtidor %d: Arreglando*****\n",2);
			printf("Surtidor %d: Arreglando*****\n",3);
			for(int j = 0; j < 4; j++) estaVacio[j] = false;
			/*Arreglamos los surtidores durante 0.7 segundos*/
			this_thread::sleep_for(chrono::milliseconds(700));
			printf("Surtidor %d: OK!\n",0);
			printf("Surtidor %d: OK!\n",1);
			printf("Surtidor %d: OK!\n",2);
			printf("Surtidor %d: OK!\n",3);
			for(int j = 0; j < 4; j++) estaVacio[j] = true;
			surtidores.signal(4);
		ocupado.signal();
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
	thread rev = thread(&revisor);	
	for(int i = 0; i < MAXCOCHES; i++){
		V[i].join();
	}
	rev.join();

}

