//********************************
// File: monitorVallas.cc
// Author: Sergio Herrero 698521
// Date: Noviembre 2016
// Coms: Implementacion del Monitor
// Compilar: g++ -std=c++11 -pthread monitorVallas.cc -c
//********************************

#include "monitorVallas.h"

/*Constructor*/
monitorValla::monitorValla(int n){
	this->precio = n;
	this->tiempoTotal = 0;
}

/* Gestiona las solicitudes de las colas de las vallas*/
void monitorValla::solicitar(char imagen[], int &tiempo, int &precio, bool &error){
	unique_lock<mutex> lck(mtx);
	error = false;
	anuncio nuevo;
	crear(nuevo, imagen, tiempo); //Creado nuevo anuncio
	colaAnuncios.push(nuevo);
	hayAnuncio.notify_one();
	precio = this->precio * tiempo; //Calculado el precio total
	int auxTiempo = tiempo; //Nos guardamos el tiempo solicitado
	tiempo = tiempoTotal;
	tiempoTotal += auxTiempo; //Actualizamos el tiempo
}

/* Devuelve el siguiente anuncio a mostrar*/
void monitorValla::cogerAnuncio(char imagen[], int &tiempo){
	unique_lock<mutex> lck(mtx); //EX.MUTUA
	while(colaAnuncios.empty()){ //Mientras no haya datos, me duermo
		hayAnuncio.wait(lck);
	}
	anuncio nuevo;
	//Sacamos de la cola y asignamos a tiempo e imagen
	tiempo = obtenerTiempo(colaAnuncios.front());
	obtenerImagen(colaAnuncios.front(), imagen);
	colaAnuncios.pop();//Desapilamos
}


