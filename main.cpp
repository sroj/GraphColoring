#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <exception>
#include "include/Graph.h"

using namespace std;

int main(int argc, const char *argv[])
{

    if (argc <= 5 && argc >= 2)
    {
        int algoritmo = 0; // 0-> Dsatur 1-> Brown
        int tmax = 15*60;

        for (int i=1; i<argc -1 ; i++)
        {
            if (strcmp("-b", argv[i]) == 0)
            {
                algoritmo = 1;
            }
            else if (strcmp("-t", argv[i]) ==0)
            {
                int t = atoi(argv[i+1]);
                if(t != 0)
                {
                    tmax = t;
                }
                else
                {
                    //throw new exception("El valor indicado para el tiempo es incorrecto");
                }
            }
            else if (strcmp("-d", argv[i]) == 0)
            {
                // Falta ver qué hacer con esto
            }
            else
            {
               //throw new exception("Uno de los argumentos es invalido");
            }
        }

        Graph grafo(argv[argc-1]);

        if (algoritmo == 0)
        {
            grafo.Dsatur(tmax);
        }
        else
        {
            grafo.Brown(tmax);
        }
    }
    else
    {
       // throw new exception("El numero de argumentos es incorrecto");
    }
    return 0;
}
