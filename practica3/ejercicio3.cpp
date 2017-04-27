//*****************************************************************
// File:    ejercicio3.cpp
// Author:  Sergio Herrero NIA:698521
// Date:    Noviembre 2016
// Coms:    Ejercicio 3 de la practica 3
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

Semaphore surtidores(4); //Semaforo que representa los cuatro surtidores
Semaphore ocupado(1); //Semaforo para que solo un proceso busque que surtidor esta disponible
Semaphore llego(1);	//Semaforo para controlar que se añaden los coches a la cola
Semaphore jefe(0);//semaforo para avisar al director de que a llegado un coche
Semaphore Terminado(1); //Semaforo para EX.MUTUA de la funcion heTerminado
Semaphore s[MAXCOCHES]; //Semaforo para controlar entrada de coches

int colaFIFO[MAXCOCHES]; //Cola FIFO de coches de llegada
int numCoches = 0; //Numero de coches que estan en la cola
bool estaVacio[4]; //Indica cual de todos los surtidores estan vacios
bool CochesAcabados[MAXCOCHES]; //Indica cuantos coches han terminado serv.
bool finDelServicio; //Indica cuando cierra la gasolinera

/* Pre: Al menos una componente del vector 'estaVacio', es 'true'
 * Post: Devuelve la componente del vector que es vacio
 */ 
int libre(){
	for(int i = 0; i < 4; i++){
		if(estaVacio[i] == true) return i;
	}
}

/* Procedimiento para avisar de los coches que han terminado */
void heTerminado(int i){
	Terminado.wait();
	int n = 0;
	CochesAcabados[i] = true;
	for(int i = 0; i < MAXCOCHES; i++){
		if(CochesAcabados[i] == true) n++;
	}
	if(n == MAXCOCHES) finDelServicio = true;
	Terminado.signal();
}

/* Sale el primer coche, y los demas se desplazan una posicion */
void avanzoCola(){
	for(int i = 0; i < numCoches; i++){
		colaFIFO[i] = colaFIFO[i+1];
	}
	numCoches--;
}

/* Procedimiento que añade el coche "id" a la cola */
void sumoCola(int id){
	colaFIFO[numCoches] = id;
	numCoches++;
}

/* Devuelve si y solo si todos los surtidores estan llenos */
bool estaLleno(){
	return (estaVacio[0]==false && estaVacio[1]==false && estaVacio[2]==false && estaVacio[3]==false);
}

/* Proceso que recorre durante un tiempo, y cuando se queda sin gasolina va a repostar. Una vez que accede
 * a un surtidor vacio, reposta, y cuando esta completo vueve a circular.
 */
void vehiculo(int j){
	int surti;
	srand(time(NULL));
	for(int i = 0; i < MAX; i++){
		//Recorre unos kilometros		
		std::this_thread::sleep_for(std::chrono::seconds(rand()%3));
		//Se pone en la fila
		llego.wait();
			sumoCola(j);
			printf("El COCHE %d ha llegado\n",j);
		llego.signal();
		jefe.signal();	//Avisa al director de que ha llegado	
		s[j].wait();	//espero a que me despierten
		//Una vez despierto, cojo el surtidor
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
		jefe.signal();
	}
	heTerminado(j); //Aviso al director de que he terminado de recorrer
}

/* Proceso que actua como barrera de paso a la gasolinera. Se encarga
 * de ir despertando a cada coche que llega a la cola
 */
void direccion(){
	bool lleno;
	while(!finDelServicio || numCoches != 0){
		ocupado.wait();
			lleno = estaLleno(); //Comprueba si esta llena la gasolinera
		ocupado.signal();	
		if(numCoches != 0 && !lleno){	//Si no esta llena
			s[colaFIFO[0]].signal(); //Despierta al primero de la lsita
			llego.wait();
				avanzoCola(); //avanza la cola en EX.MUTUA
			llego.signal();
			jefe.signal(); //Se avisa de que "algo" ha pasado
		}
	}
}

/* Inicia el vector estaVacio con todo valores 'true' */
void iniciarVector(){
	for(int i = 0; i < 4; i++)
		estaVacio[i] = true;
}

/* Inicia el vector de Semaforo de Coches */
void iniciarVectorCoches(){
	for(int i = 0; i < MAXCOCHES; i++)
		s[i].setInitValue(0);
}
int main(){
	iniciarVector(); //Iniciamos los surtidores; todos estan libres
	iniciarVectorCoches(); //Iniciamos el vector de Semaforos de los coches
	thread V[MAXCOCHES];	
	for(int i = 0; i < MAXCOCHES; i++){
		V[i] = thread(&vehiculo, i);
	}
	thread rev = thread(&direccion);	
	for(int i = 0; i < MAXCOCHES; i++){
		V[i].join();
	}
	rev.join();
	cout<<"FIN!"<<endl;

}

