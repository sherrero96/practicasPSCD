//*****************************************************************
// File:    ejercicio2.cpp
// Author:  Sergio Herrero Barco 698521
// Date:    1 de Noviembre 2016
// Coms:    Ejercicio 2 de la práctica 2
//*****************************************************************

#include <iostream>
#include <atomic>
#include <time.h>
#include <thread>
using namespace std;

const int ND = 200;	//num. de datos
const int NP = 10;  //num. de procesos
const bool MOSTRAR   =   1; //Variable que permitira mostrar todas las componentes del vector

int d[ND]; //Vector de datos enteros
int maxGlobal;
bool hanTerminado[NP]; //Vector que determina si han terminado o no los procesos
atomic_flag ocup = ATOMIC_FLAG_INIT; //Variable ¡atomica! para que los procesos actuen en exclusion mutua

/* Funcion que calcula el valor maximo en el vector v, desde la posicion i1 hasta i2
 * y lo pone en la direccion de maxGlobal. El num sirve para identificar el proceso que lo realiza*/
void maxTrozo(int *v, int i1, int i2, int num){
	atomic<bool> no_ent;
    /* INICIO SECCION NO CRITICA */
    int max = v[i1];
	for(i1++; i1 < i2; i1++){
		if(v[i1] > max) max = v[i1];
	}
    no_ent = ocup.test_and_set(); //Asignamos el valor que tenia ocup a no_ent, y ocup lo ponemos a true <ATOMICO>
    while(no_ent){
       no_ent = ocup.test_and_set(); //Si no se puede entrar, vamos asignando todo el rato el valor hasta que ocup sea falso <ATOMICO>
    }
    /* INICIO SECCION CRITICA */
    /* Estamos en exclusion mutua, ocup = true, por lo que ningun proceso esta en su seccion critica */
    if(max > maxGlobal) maxGlobal = max;
	hanTerminado[num] = true; //Ponemos el valor true en el vector
    ocup.clear(); //La variable ya no esta ocupada, por lo que ocup = false
}

/* Procedimiento que espera que terminen todos y muestra por pantalla el maximo de todo el vector*/
void informador(){
	for(int i = 0; i < NP; i++){
		while(hanTerminado[i] == false){
			; //ESPERA ACTIVA
		}
	}
	cout<<"MAX = ";
	cout<<maxGlobal<<endl;

}

/* Funcion que inicializa el vector con valores aleatorios*/
void inicializar(int v[]){
	srand(time(NULL));
    for(int i = 0; i < ND; i++){
		v[i] = rand();
	}
	
}

/* Funcion que inicia el vector de booleanos a falso*/
void iniciarBooleano(bool v[]){
	for(int i = 0; i < NP; i++)
		v[i] = false;
}



int main(){
	thread P[NP]; //Vector de threads
	inicializar(d);
	iniciarBooleano(hanTerminado);
	int principio, fin;
	
	/*SI LA VARIABLE MOSTRAR ES ACTIVA, MOSTRARA POR PANTALLA EL VECTOR*/
	/*********************************/
	if(MOSTRAR){
		for(int i = 0; i < ND; i++){
			cout<<d[i]<<" ";
		}
		cout<<endl;
	}
	/*********************************/
	
	for(int i = 0; i < NP; i++){
		principio = i* (ND/NP); //Calculamos el principio del vector segun la cantidad de datos
		fin = principio + (ND/NP);
		P[i] = thread(&maxTrozo, ref(d), principio, fin,i);
	}
	thread infor(&informador); //Invocamos tambien el thread que informara por pantalla
	
	/*Esperamos que todos hayan terminado*/
	for(int i =0; i < NP; i++){
		P[i].join();
	}
	infor.join();
	cout<<"Fin!"<<endl;
	
}
