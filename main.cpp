#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"
#include <ctime>
#include <string>

using namespace std;

enum algoritmo_t {
    dsatur, brown
};

int main(int argc, char **argv) {

    if (argc >= 2 && argc <= 5) {
        algoritmo_t algoritmo = dsatur; // 0-> Dsatur 1-> Brown
        int tmax = 15 * 60;

        for (int i = 1; i < argc - 1; i++) {
            if (strcmp("-b", argv[i]) == 0) {
                algoritmo = brown;
            } else if (strcmp("-t", argv[i]) == 0) {
                int t = atoi(argv[i + 1]);
                if (t != 0) {
                    tmax = t;
                } else {
                    throw string("El valor indicado para el tiempo es incorrecto");
                }
            } else if (strcmp("-d", argv[i]) != 0) {
                throw string("Uno de los argumentos es invalido");
            }
        }

        try {
            Graph grafo(argv[argc - 1]);
            if (algoritmo == dsatur) {
                grafo.Dsatur(tmax);
            } else {
                grafo.Brown(tmax);
            }
            grafo.printCurrentColoring();
        } catch (string mensaje) {
            cout << mensaje;
            return EXIT_FAILURE;
        }
    } else {
        cout << ("El numero de argumentos es incorrecto");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
