monitor:
	g++ -std=c++11 -pthread monitorVallas.cc -c
anuncio:
	g++ -std=c++11 -pthread anuncio.cc -c
simulacion:
	g++ -std=c++11 -pthread -L/usr/X11R6/lib -lm -lpthread -lX11 anuncio.o monitorVallas.o simulacionVallas.cc -o simulacionVallas
