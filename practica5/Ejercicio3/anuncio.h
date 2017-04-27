//********************************
// File: anuncio.h
// Author: Sergio Herrero 698521
// Date: Noviembre 2016
// Coms: Interfaz del TAD anuncio
//********************************

#include <iostream>
#include <cstring>

using namespace std;

/* El TAD anuncio representa un par de (imagen, tiempo)
 * que consiste en una imagen a mostrar, y el tiempo que
 * quiere que se muestre*/
struct anuncio{
	private:
		char im[100];
		int ti;
	public:
		friend void crear(anuncio &a, char imagen[], int tiempo);
		friend void obtenerImagen(anuncio &a, char imagen[]);
		friend int obtenerTiempo(anuncio &a);
};

/* Funcion para crear una anuncio*/
void crear(anuncio &a, char imagen[], int tiempo);
/* Devuelve en imagen, la imagen correspondiente al anuncio a*/
void obtenerImagen(anuncio &a, char imagen[]);
/*Devuelve el tiempo que corresponde al anuncio a*/
int obtenerTiempo(anuncio &a);
