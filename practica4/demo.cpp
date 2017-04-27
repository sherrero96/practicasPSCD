//*****************************************************************
// File:    demo.cpp
// Author:  PSCD-Unizar
// Date:    11/11/15
// Coms:    Programación de Sistemas Concurrentes y 
//          Distribuidos Curso 2015-2016.
//          Demostración de uso de la librería "ImageDownloader" para
//          la descarga de imágenes de Internet y de la librería "CImg"
//          para la visualización de imágenes 
//          Compilar mediante "make"
//*****************************************************************

#include <iostream>
#include <string>
#include "ImageDownloader.h"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

int main()
{
	// Tamaños de ventana para las vallas publicitarias
	const int PRINCIPAL_WIDTH = 800;
	const int PRINCIPAL_HEIGHT = 800;
	const int SECUNDARIA_WIDTH = 800;
	const int SECUNDARIA_HEIGHT = 400;

	// Creamos el objeto para descargar imágenes
	ImageDownloader downloader;
	// Descargamos una imagen de Internet
	char URL[500] = "http://assets2.motherboard.tv/content-images/contentimage/no-slug/7703bcea71c8096ea7c18addc28e8a15.jpg";
	char path[100] = "imgs/image1.jpg";
	downloader.downloadImage(URL, path);
	
	// Creamos las vallas publicitarias con las imágenes adecuadas
	// VALLA PRINCIPAL
	cimg_library::CImg<unsigned char> img_principal(path);
	cimg_library::CImgDisplay valla_principal(img_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH), "VALLA PRINCIPAL");
	valla_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH);
	valla_principal.move(0,0); // Esquina superior izquierda

	// VALLA SECUNDARIA 1
	cimg_library::CImg<unsigned char> img_secundaria1("imgs/image2.jpg");
	cimg_library::CImgDisplay valla_secundaria1(img_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT), "VALLA SECUNDARIA 1");
	valla_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT);
	//valla_secundaria1.move(??????); // move a esquina derecha superior
	//valla_secundaria1.wait(??????); // mostrar durante ?????? milisegundos


	// VALLA SECUNDARIA 2
	cimg_library::CImg<unsigned char> img_secundaria2("imgs/image3.jpg");
	cimg_library::CImgDisplay valla_secundaria2(img_secundaria2.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT), "VALLA SECUNDARIA 2");
	valla_secundaria2.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT);
	//valla_secundaria2.move(??????); // move a esquina derecha inferior
	//valla_secundaria2.wait(??????); // mostrar durante ?????? milisegundos

	// Mostramos la valla principal durante 5 segundos
	valla_principal.wait(5000);
	// Cambiamos la imagen mostrada en la valla principal
	img_principal.assign("imgs/image4.jpg");
	valla_principal.render(img_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH));
	valla_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH);
	valla_principal.move(0,0); // Esquina superior izquierda
	valla_principal.paint(); // Repintar nueva imagen en la valla principal
	// Esperamos otros 5 segundos
	valla_principal.wait(5000);
	
	return 0;
}
