//*****************************************************************
// File:   ejercicio_4.cpp
// Author: Sergio Herrero Barco NIA:698521
// Date:   Octubre 2016
// Coms:   Ejercicio 4 de la practica 1 de PSCD
//*****************************************************************

#include <iostream>
#include <iomanip>
#include <thread>
#include <cmath>

using namespace std;
const int MAX = 10; //Tamanyo del vector



/*Inicializa el vector rellenandolo con 100 valores reales aleatorios*/
void iniciarVector(double v[]){
    srand(time(NULL)); //Semilla
    for(int i = 0; i < MAX; i++){
        v[i] = rand();
    }
}

/*Funcion que devuelve en med la media de los valores del vector*/
void mediaAritmetica(double *v, double *med){
    *med = 0;
    for(int i = 0; i < MAX; i++){
        *med += v[i];
    }
   *med = *med / MAX;
}

/*Funcion que devuelve en mini, el valor minimo de todos los valores del vector v*/
void minimo(double *v, double *mini){
   *mini = v[0];
    for(int i = 1; i < MAX; i++){
        if(*mini > v[i])
            *mini = v[i];
    }
}

/*Funcion que devuelve en maxi, el valor minimo de todos los valores del vector v*/
void maximo(double *v, double *maxi){
    *maxi = v[0];
    for(int i = 1; i < MAX; i++){
        if(*maxi < v[i])
            *maxi = v[i];
    }
}
/* Devuelve la varianza de los datos del vector v
 * Siguiendo la formula de la desviacion tipica:
 * Desv = sqrt(SUM ALFA EN [0,MAX-1].((v[i]-media)²)/MAX)
 */
void desviacion(double *v, double *desv, double promedio){       
        double varianza = 0;
        for (int i = 0; i < MAX; i++) {
                varianza = varianza + (v[i] - promedio) * (v[i] - promedio);
        }
        varianza = sqrt(varianza/MAX);
        *desv = varianza;
} 



int main(){
    double v[MAX];
    iniciarVector(v); //Funcion que inica el vector con MAX numeros aleatorios
    double media, maxi, mini, sigma;
    thread T[4];
    
    T[0] = thread(&mediaAritmetica, ref(v), &media); //Lanzamos el proceso que calcula la media
    T[1] = thread(&maximo, ref(v), &maxi); //Lanzamos el proceso que devuelve el maximo numero
    T[2] = thread(&minimo, ref(v), &mini); //Lanzamos el proceso que devuelve el minimo numero
    
    
    /*Esperamos que termine el proceso T[0] que es el proceso que calcula la media*/
    T[0].join();
    /*Ahora que a terminado, podemos lanzar el proceso que calcula la desviacion tipica T[3]*/
    T[3] = thread(&desviacion, ref(v), &sigma, media);
    
    /*Ya podemos esperar a terminar todos los procesos*/
    for(int i = 1; i < 4; i++){
        T[i].join();
    }
    
    /*Muestra de datos por pantalla*/
    cout<<"#datos:\t"<<MAX<<endl;
    cout<<"media:\t"<<media<<endl;
    cout<<"max:\t"<<maxi<<endl;
    cout<<"min:\t"<<mini<<endl;
    cout<<"sigma:\t"<<sigma<<endl;
}