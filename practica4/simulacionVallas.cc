//********************************
// File: simulacionVallas.cc
// Author: Sergio Herrero 698521
// Date: Noviembre 2016
// Coms: Implementacion de la simulacion
// Compilar: g++ -std=c++11 -pthread -L/usr/X11R6/lib -lm -lpthread -lX11 anuncio.o monitorVallas.o simulacionVallas.cc -o simulacionVallas
//********************************

#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <string>
#include <cstring>
#include "CImg.h"
#include "monitorVallas.h"


using namespace std;
using namespace cimg_library;

// Tamaños de ventana para las vallas publicitarias
	const int PRINCIPAL_WIDTH = 480;
	const int PRINCIPAL_HEIGHT = 540;
	const int SECUNDARIA_WIDTH = 480;
	const int SECUNDARIA_HEIGHT = 270;
	
/* Monitores para controlar las vallas*/
monitorValla mtGrande(50), mtPeque(10);

/* Proceso que simula un cliente que quiere mostrar una
 * imagen en una valla publcitaria grande
 */
void cliente(int iden){
	int n = 2; //Numero de veces que van a solicitar la imagen
	bool error;
	char imagen[100];
	while(n != 0){	
		/* Cada cliente solicita solo su imagen*/
		if(iden == 0) strcpy(imagen, "image1.jpg");
		else if(iden == 1) strcpy(imagen, "image2.jpg");
		else strcpy(imagen, "image3.jpg");
		int tamanyo = 0, tiempo = 10, precio;
		/* Solicitamos la imagen*/
		mtGrande.solicitar(imagen, tiempo, precio, error);
		if(error){ //Significa que ha habido un problema, volvemos a solicitar
			mtGrande.solicitar(imagen, tiempo, precio, error);
		}
		printf("CLIENTE %d [VALLA GRANDE]: Su imagen aparecera en %d segundos\n",iden, tiempo);
		//Esperamos a volver a hacerlo
		std:this_thread::sleep_for(std::chrono::seconds(10));
		n--;
	}
}

/* Proceso que simula un cliente que quiere mostrar una
 * imagen en una valla publcitaria pequeña
 */
void cliente2(int iden){
	int n = 2;//Numero de veces que van a solicitar la imagen
	bool error;
	char imagen[100];
	while(n != 0){	
		/* Cada cliente solicita solo su imagen*/
		if(iden == 3) strcpy(imagen, "image2.jpg");
		else if(iden == 4) strcpy(imagen, "image3.jpg");
		else strcpy(imagen, "image4.jpg");
		int tamanyo = 0, tiempo = 5, precio;
		/* Solicitamos la imagen */
		mtPeque.solicitar(imagen, tiempo, precio, error);
		if(error){ //Significa que ha habido un problema, volvemos a solicitar
			mtPeque.solicitar(imagen, tiempo, precio, error);
		}
		printf("CLIENTE %d [VALLA PEQUE]: Su imagen aparecera en %d segundos\n",iden, tiempo);
		//Esperamos a volver a hacerlo
		std:this_thread::sleep_for(std::chrono::seconds(10));
		n--;
	}
}





/* Proceso que simula la valla grande */
void vallaGrande(){
	int n = 10;
	while(n != 0){
		char imagen[100];
		int tiempo;
		/* Si no hay ninguna imagen que mostrar, el monitor nos bloqueara */
		/* Si hay alguna imagen, pondra la ruta en 'imagen' y el tiempo*/
		mtGrande.cogerAnuncio(imagen, tiempo);
		// Creamos las vallas publicitarias con las imágene adecuadas
		// VALLA PRINCIPAL
	cimg_library::CImg<unsigned char> img_principal(imagen);
	cimg_library::CImgDisplay valla_principal(img_principal.resize(PRINCIPAL_WIDTH,PRINCIPAL_WIDTH),"VALLA PRINCIPAL");
	valla_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH);
	valla_principal.move(0,0); // Esquina superior izquierda
	valla_principal.wait(tiempo*1000);
	n--;
	}
}


/* Proceso que simula la valla pequeña */
void vallaPeque1(){
	int n = 5;
	while(n != 0){
		char imagen[100];
		int tiempo;
		/* Si no hay ninguna imagen que mostrar, el monitor nos bloqueara */
		/* Si hay alguna imagen, pondra la ruta en 'imagen' y el tiempo*/
		mtPeque.cogerAnuncio(imagen, tiempo);
		cimg_library::CImg<unsigned char> img_secundaria1(imagen);
	cimg_library::CImgDisplay valla_secundaria1(img_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT),"VALLA SECUNDARIA 1");
	valla_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT);
	valla_secundaria1.move(PRINCIPAL_WIDTH,0); // Esquina superior izquierda
	valla_secundaria1.wait(tiempo*1000);
	n--;
	}
}

/* Proceso que simula la valla pequeña */
void vallaPeque2(){
	int n = 5;
	while(n!=0){
		char imagen[100];
		int tiempo;
		/* Si no hay ninguna imagen que mostrar, el monitor nos bloqueara */
		/* Si hay alguna imagen, pondra la ruta en 'imagen' y el tiempo*/
		mtPeque.cogerAnuncio(imagen, tiempo);
		cimg_library::CImg<unsigned char> img_secundaria1(imagen);
	cimg_library::CImgDisplay valla_secundaria1(img_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT),"VALLA SECUNDARIA 2");
	valla_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT);
	valla_secundaria1.move(PRINCIPAL_WIDTH,SECUNDARIA_HEIGHT);
	valla_secundaria1.wait(tiempo*1000);
	n--;
	}
}

int main(){
	const int num_clientes = 20;
	thread CLIENTE[num_clientes];
	thread PantallaGrande = thread(&vallaGrande); //Lanzamos las tres vallas
	thread PantallaPeque1 = thread(&vallaPeque1);
	thread PantallaPeque2 = thread(&vallaPeque2);
	for(int i = 0; i < num_clientes/2; i++){
		CLIENTE[i] = thread(&cliente,i);
	}
	for(int i = num_clientes/2; i < num_clientes+1; i++){
		CLIENTE[i] = thread(&cliente2,i);
	}
	
	PantallaGrande.join();
	PantallaPeque1.join();
	PantallaPeque2.join();
	for(int i = 0; i < 6; i++){
		CLIENTE[i].join();
	}
	cout<<"FIN!"<<endl;
}










