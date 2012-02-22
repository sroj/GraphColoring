
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"
#include <ctime>
#include <string>

using namespace std;

enum algoritmo_t
{
    dsatur, brown
};

int main(int argc, char **argv)
{
    if (argc >= 2 && argc <= 5)
    {
        algoritmo_t algoritmo = dsatur;
        int tmax = 15 * 60;

        for (int i = 1; i < argc - 1; i++)
        {
            if (strcmp("-b", argv[i]) == 0)
            {
                algoritmo = brown;
            }
            else if (strcmp("-t", argv[i]) == 0)
            {
                int t = atoi(argv[i + 1]);
                if (t != 0)
                {
                    tmax = t;
                }
                else
                {
                    throw string("El valor indicado para el tiempo es incorrecto");
                }
            }
            else if (strcmp("-d", argv[i]) != 0)
            {
                throw string("Uno de los argumentos es invalido");
            }
        }

        try
        {
            int contadorArchivos=0;
            double sumaTiempo=0;
            int coloresMin = 0;
            int sumaColores=0;
            int cantExcedido=0;
            double executionTime;
            for(int i=1; i<=60; i++)
            {
                char nombreArchivo[12];
                sprintf (nombreArchivo, "%d", i);
                Graph grafo(nombreArchivo);
                contadorArchivos++;
                //Graph grafo(argv[argc - 1]);
                if (algoritmo == dsatur)
                {
                    executionTime = grafo.Dsatur(tmax);
                }
                else
                {
                    executionTime = grafo.Brown(tmax);

                }
                if(executionTime !=-1)
                {
                    coloresMin = grafo.getNumberOfColors();
                    sumaTiempo += executionTime;
                    sumaColores += coloresMin;
                    grafo.printOutput(cout, executionTime);
                }
                else
                {
                    cantExcedido++;
                }


                if(contadorArchivos==3)
                {
                    ofstream archivoGenerado;
                    strcat(nombreArchivo,".out");
                    archivoGenerado.open (nombreArchivo);
                    if (archivoGenerado.is_open())
                    {
                        archivoGenerado << "colores promedio: " << (double)sumaColores/3.0 << "\n";
                        archivoGenerado << "tiempo promedio: " << (double)sumaTiempo/3.0 << "\n";
                        archivoGenerado << "cuantos con time out: " << cantExcedido << "\n";
                        archivoGenerado.close();
                    }
                    else
                    {
                        throw string("archivoooo2");
                    }
                    contadorArchivos=0;
                    sumaColores =0;
                    sumaTiempo=0;
                    cantExcedido=0;
                }

            }

        }
        catch (string mensaje)
        {
            cout << mensaje;
            return EXIT_FAILURE;
        }
    }
    else
    {
        cout << ("El numero de argumentos es incorrecto");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
