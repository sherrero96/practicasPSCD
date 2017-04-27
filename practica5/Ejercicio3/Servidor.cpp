//*****************************************************************
// File:   Servidor.cpp
// Author: Sergio Herrero Barco 698521
// Date:   Enero 2017
// Coms:   Ejemplo de servidor de vallas publicitarias con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <thread>
#include "CImg.h"
#include "monitorVallas.h"
#include "Socket.h"
#include "ImageDownloader.h"


using namespace std;
using namespace cimg_library;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const int NUM_VENTANAS = 3;
char path[NUM_VENTANAS][100] = {"image1.jpg","image2.jpg","image3.jpg"};

// Tamaños de ventana para las vallas publicitarias
	const int PRINCIPAL_WIDTH = 480;
	const int PRINCIPAL_HEIGHT = 540;
	const int SECUNDARIA_WIDTH = 480;
	const int SECUNDARIA_HEIGHT = 270;
	
monitorValla mtGrande(50), mtPeque(10); //Monitores para controlar y administrar las vallas

/* Funcion que trocea el mensaje buffer y asigna la URL pedida, la Pantalla elegida, y el tiempo que se mostrara.
 * El mensaje sigue el patron:
 * URL,TamañoDeLaPantalla,TiempoEnSegundos
 */
void obtenerDatos(string buffer,int numDatos, char URL[], char Tamanyo[], char Tiempo[]){
	int i = 0;
	while(buffer[i] != ','){
		URL[i] = buffer[i];
		i++;
	}
	URL[i] = '\0'; i++;
	int j = 0;
	while(buffer[i] != ','){
		Tamanyo[j] = buffer[i];
		i++; j++;
	}
	Tamanyo[j] = '\0';
	i++;
	j = 0;
	while(i != numDatos){
		Tiempo[j] = buffer[i];
		i++;j++;
	}
	Tiempo[j] = '\0';
}

/* Proceso que simula la valla grande */
void vallaGrande(int xpos,int ypos){
	while(1){
		char imagen[100];
		int tiempo;
		/* Si no hay ninguna imagen que mostrar, el monitor nos bloqueara */
		/* Si hay alguna imagen, pondra la ruta en 'imagen' y el tiempo*/
		mtGrande.cogerAnuncio(imagen, tiempo);
		ImageDownloader downloader;
		int error = downloader.downloadImage(imagen, path[0]);
		cout<<imagen<<","<<tiempo<<endl;
		// Creamos las vallas publicitarias con las imágene adecuadas
		// VALLA PRINCIPAL
		if(error != -1){
			cimg_library::CImg<unsigned char> img_principal(path[0]);
			cimg_library::CImgDisplay valla_principal(img_principal.resize(PRINCIPAL_WIDTH,PRINCIPAL_WIDTH),"VALLA PRINCIPAL");
			valla_principal.resize(PRINCIPAL_WIDTH,SECUNDARIA_WIDTH);
			valla_principal.move(xpos,ypos); // Esquina superior izquierda
			valla_principal.wait(tiempo*1000);
		}
	}
}

/* Proceso que simula la valla pequeña */
void vallaPeque(int xpos, int ypos){
	while(1){
		char imagen[100];
		int tiempo;
		/* Si no hay ninguna imagen que mostrar, el monitor nos bloqueara */
		/* Si hay alguna imagen, pondra la ruta en 'imagen' y el tiempo*/
		mtPeque.cogerAnuncio(imagen, tiempo);
		ImageDownloader downloader;
		int error = downloader.downloadImage(imagen, path[1]);
		cout<<imagen<<","<<tiempo<<endl;
		if(error != -1){
			cimg_library::CImg<unsigned char> img_secundaria1(path[1]);
			cimg_library::CImgDisplay valla_secundaria1(img_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT),"VALLA SECUNDARIA");
			valla_secundaria1.resize(SECUNDARIA_WIDTH,SECUNDARIA_HEIGHT);
			valla_secundaria1.move(xpos,ypos); // Esquina superior izquierda
			valla_secundaria1.wait(tiempo*1000);
		}
	}
}

/* Funcion que atiende los servicios de los clientes que solicitan mostrar su imagen*/
void atiende_servicio(Socket socket, int socket_fd){
		char MENS_FIN[]="END OF SERVICE";
		int client_fd = socket.Accept();
		if(client_fd == -1) { //Si no hemos podido aceptar la conexion
			cerr << "Error en el accept: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		// Buffer para recibir el mensaje
		int length = 100;
		string buffer;
	
		bool out = false; // Inicialmente no salir del bucle
		while(!out) {
			// Recibimos el mensaje del cliente
			int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
			if(rcv_bytes == -1) { //Si no se ha podido recibir bien los datos
				cerr << "Error al recibir datos: " << strerror(errno) << endl;
				// Cerramos los sockets
				socket.Close(client_fd);
				socket.Close(socket_fd);
			}	
	
			// Si recibimos "END OF SERVICE" --> Fin de la comunicación
			if(buffer == MENS_FIN){
				out = true; // Salir del bucle
			} else {
				char URL[100], Tamanyo[2], Tiempo[100];
				obtenerDatos(buffer, rcv_bytes, URL, Tamanyo, Tiempo);
				cout<<"Cliente: "<<client_fd<<" tiene por delante "<<mtGrande.cola()<<" imagenes."<<endl;
				// Creamos el objeto para descargar imágenes
				int tiempo = atoi(Tiempo);
				int precio; bool error;
				if(atoi(Tamanyo) == 1) {
					mtGrande.solicitar(URL,tiempo, precio, error);
				}
				else {
					mtPeque.solicitar(URL, tiempo, precio, error);
				}

				string mensaje = "Su imagen empezara en ";
				mensaje += to_string(tiempo);
				mensaje += " segundos y costara " + to_string(precio) + " euros\n";
				int send_bytes = socket.Send(client_fd, mensaje);

		    	if(send_bytes == -1){
					cerr << "Error al enviar datos: " << strerror(errno) << endl;
					// Cerramos el socket
					socket.Close(client_fd);
					exit(1);
				}
			}
		}

		// Cerramos el socket del cliente
	    int error_code = socket.Close(client_fd);
    	if(error_code == -1){
    		cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
    	}
}



//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	if(argc != 2) {cout<<"Error en el numero de parametros"<<endl;}
	else{
		char MENS_FIN[]="END OF SERVICE";
		// Puerto donde escucha el proceso servidor
  	  int SERVER_PORT = atoi(argv[1]);
	
		// Creación del socket con el que se llevará a cabo
		// la comunicación con el servidor.
		Socket socket(SERVER_PORT);

		// Bind 
		int socket_fd =socket.Bind();
		if (socket_fd == -1) {
			cerr << "Error en el bind: " << strerror(errno) << endl;
			exit(1);
		}
	
		// Listen
	    int max_connections = 10; //Permitimos hasta 10 conexiones a la vez
		int error_code = socket.Listen(max_connections);
		thread CLIENTES[max_connections];
		if(error_code == -1) {
			cerr << "Error en el listen: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		for(int i = 0; i < max_connections; i++){
			CLIENTES[i] = thread(&atiende_servicio, socket,socket_fd);
		}
		thread PantallaGrande = thread(&vallaGrande,0,0); //Lanzamos las tres vallas
		thread PantallaPeque1 = thread(&vallaPeque,PRINCIPAL_HEIGHT,0);
		thread PantallaPeque2 = thread(&vallaPeque,PRINCIPAL_HEIGHT,PRINCIPAL_WIDTH);
		for(int i = 0; i < max_connections; i++){
			CLIENTES[i].join();
		}
		PantallaGrande.join();
		PantallaPeque1.join();
		PantallaPeque2.join();

    	// Cerramos el socket del servidor
    	error_code = socket.Close(socket_fd);
    	if(error_code == -1){
    		cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    	}
		// Mensaje de despedida
		cout << "Bye bye" << endl;
		
    	return error_code;
	}
}




