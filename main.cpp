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
                if (t > 0)
                {
                    tmax = t;
                    i++;
                }
                else
                {
                    cout << "El valor de tiempo indicado es invalido\n";
                    cout << "Sintaxis correcta: gsc1 [opciones] <instancia>\n";
                    return EXIT_FAILURE;
                }
            }
            else if (strcmp("-d", argv[i]) != 0)
            {
                cout << "Sintaxis correcta: gsc1 [opciones] <instancia>\n";
                return EXIT_FAILURE;
            }
        }

        try
        {

            double executionTime;


            Graph grafo(argv[argc - 1]);
            if (algoritmo == dsatur)
            {
                executionTime = grafo.Dsatur(tmax);
            }
            else
            {
                executionTime = grafo.Brown(tmax);

            }
            if (executionTime !=-1)
            {
                grafo.printOutput(cout, executionTime);
            }
            else
            {
                cout << "El tiempo de ejecucion permitido fue excedido\n";
                return EXIT_FAILURE;
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
        cout << ("El numero de argumentos es incorrecto\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



