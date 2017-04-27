//********************************
// File: monitorVallas.h
// Author: Sergio Herrero 698521
// Date: Noviembre 2016
// Coms: Interfaz del Monitor
//********************************

#ifndef MONITOR_H
#define MONITOR_H

#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <queue>
#include <cstring>
#include <iostream>
#include "anuncio.h"

using namespace std;


/* Monitor para controlar la gestion concurrente de pantallas
 * publicitarias */
class monitorValla{
	private:
		mutex mtx; //Mutex para asegurar EX.MUTUA en todas las operaciones
		int precio; //Precio por segundo de la valla
		int tiempoTotal; //Tiempo total de todos los anuncios
		queue<anuncio> colaAnuncios; //Cola donde guardar las imagenes
		condition_variable hayAnuncio;
		
	public:
		monitorValla(int n); //Constructor
		/* Gestiona las solicitudes de las colas de las vallas*/
		void solicitar(char imagen[], int &tiempo, int &precio, bool &error);
		/* Devuelve el siguiente anuncio a mostrar*/
		void cogerAnuncio(char imagen[], int &tiempo);
		/* Devuelve el tamanyo de la cola de anuncios */
		int cola();

		
};


#endif
