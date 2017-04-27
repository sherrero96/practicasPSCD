//*****************************************************************
// File:   Servidor.cpp
// Author: Sergio Herrero Barco 698521
// Date:   Enero 2017
// Coms:   Ejemplo de servidor con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

//-------------------------------------------------------------
// Cuenta el número de vocales existentes en un mensaje
// PRE: 
// POST: Devuelve el número de vocales existentes en el mensaje 'message'
int cuentaVocales(string message) {
	int count = 0;

	for(int i=0; i < message.length(); i++) {

		switch(message[i]) {
			case 'a': case 'A': 
			case 'e': case 'E': 
			case 'i': case 'I': 
			case 'o': case 'O': 
			case 'u': case 'U': 
						count++;
					  	break;
		}
	}

	return count;
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
	    int max_connections = 1;
		int error_code = socket.Listen(max_connections);
		if(error_code == -1) {
			cerr << "Error en el listen: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		// Accept
		int client_fd = socket.Accept();
		if(client_fd == -1) {
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
			if(rcv_bytes == -1) {
				cerr << "Error al recibir datos: " << strerror(errno) << endl;
				// Cerramos los sockets
				socket.Close(client_fd);
				socket.Close(socket_fd);
			}

			cout << "Mensaje recibido: '" << buffer << "'" << endl;
	
			// Si recibimos "END OF SERVICE" --> Fin de la comunicación
			if(buffer == MENS_FIN){
				out = true; // Salir del bucle
			} else {
				// Contamos las vocales recibidas en el mensaje anterior
				int num_vocales = cuentaVocales(buffer);

				// Enviamos la respuesta
				string resp = to_string(num_vocales);
			
				int send_bytes = socket.Send(client_fd, resp);
				if(send_bytes == -1) {
					cerr << "Error al enviar datos: " << strerror(errno) << endl;
					// Cerramos los sockets
					socket.Close(client_fd);
					socket.Close(socket_fd);
					exit(1);
				}
			}
		}

		// Cerramos el socket del cliente
	    error_code = socket.Close(client_fd);
    	if(error_code == -1){
    		cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
    	}

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
