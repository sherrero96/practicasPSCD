//*****************************************************************
// File:   Cliente.cpp
// Author: Sergio Herrero Barco 698521
// Date:   Enero 2017
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main(int argc, char *argv[]) {
	const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    if(argc != 3) {cout<<"Error en el numero de parametros"<<endl;}
    else{
    	
    	string SERVER_ADDRESS = argv[1];
    	int SERVER_PORT = atoi(argv[2]);
		cout<<SERVER_ADDRESS<<endl;

		// Creación del socket con el que se llevará a cabo
		// la comunicación con el servidor.
		Socket socket(SERVER_ADDRESS, SERVER_PORT);

    	// Conectamos con el servidor. Probamos varias conexiones
		const int MAX_ATTEMPS = 10;
		int count = 0;
		int socket_fd;
		do {
			// Conexión con el servidor
	    	socket_fd = socket.Connect();
	    	count++;
	
	    	// Si error --> esperamos 1 segundo para reconectar
	    	if(socket_fd == -1){
	    	    this_thread::sleep_for(chrono::seconds(1));
	    	}
    	} while(socket_fd == -1 && count < MAX_ATTEMPS);

    	// Chequeamos si se ha realizado la conexión
    	if(socket_fd == -1){
    		return socket_fd;
    	}

    
	    string mensaje;
		do{
			// Leer mensaje de la entrada estandar
			cout << "Frase para contar las vocales: ";
			getline(cin, mensaje);
			// Enviamos el mensaje
		    int send_bytes = socket.Send(socket_fd, mensaje);

		    if(send_bytes == -1){
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos el socket
				socket.Close(socket_fd);
				exit(1);
			}

			if(mensaje != MENS_FIN){
			    // Buffer para almacenar la respuesta
			    string buffer;

			    // Recibimos la respuesta del servidor  
			    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);

			    // Mostramos la respuesta
			    cout << "Mensaje enviado: '" << mensaje << "'" << endl;
			    cout << "Numero de vocales: " << buffer << endl;
			}
		} while(mensaje != MENS_FIN);

	    // Cerramos el socket
	    int error_code = socket.Close(socket_fd);
	    if(error_code == -1){
			cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	    }

	    return error_code;
	}
}
