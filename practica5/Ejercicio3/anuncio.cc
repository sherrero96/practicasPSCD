//********************************
// File: anuncio.cpp
// Author: Sergio Herrero 698521
// Date: Noviembre 2016
// Coms: Implemenacion del TAD anuncio
// Compilar: g++ -std=c++11 -pthread anuncio.cc -c
//********************************

#include "anuncio.h"

/* Funcion para crear una anuncio*/
void crear(anuncio &a, char imagen[], int tiempo){
	strcpy(a.im, imagen);
	a.ti = tiempo;
}

/* Devuelve en imagen, la imagen correspondiente al anuncio a*/
void obtenerImagen(anuncio &a, char imagen[]){
	strcpy(imagen, a.im);
}

/*Devuelve el tiempo que corresponde al anuncio a*/
int obtenerTiempo(anuncio &a){
	return a.ti;
}
