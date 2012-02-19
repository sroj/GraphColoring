#ifndef GRAPH_H
#define	GRAPH_H

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "GraphNode.h"
#include "CompareDegreeFunctor.h"
#include "CompareLabelFunctor.h"

using namespace std;

class Graph
{
public:

    Graph(const char path[])
    {
        int u;
        int v;
        string linea;
        ifstream instancia(path);
        char* token;
        bool primeraLineaLeida = false;

        if (instancia.is_open())
        {
            while (instancia.good())
            {
                getline(instancia, linea);
                char copia [linea.size() + 1];
                strcpy(copia, linea.c_str());

                token = strtok(copia, " ");
                while (token != NULL)
                {
                    if (strcmp(token, "e") == 0)
                    {
                        if (!primeraLineaLeida)
                            throw string("Formato de archivo invalido");
                        token = strtok(NULL, " ");
                        u = atoi(token);
                        token = strtok(NULL, " ");
                        v = atoi(token);
                        addArc(u, v);
                        break;
                    }
                    else if (strcmp(token, "c") == 0)
                    {
                        break;
                    }
                    else if (strcmp(token, "p") == 0)
                    {
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");
                        numNodes = atoi(token);
                        token = strtok(NULL, " ");
                        numEdges = atoi(token);
                        nodesArray = new GraphNode* [numNodes];
                        adjacencyArray = new vector<GraphNode*>* [numNodes];
                        nodesDegreeSortedArray = new GraphNode* [numNodes];
                        initializeNodesArrays();
                        initializeAdjancencyArray();
                        primeraLineaLeida = true;
                        break;
                    }
                    token = strtok(NULL, " ");
                }
            }
            instancia.close();
            sortNodesByDegree();
            sortAdjacentNodes();
        }
        else
        {
            throw string("No se pudo abrir el archivo");
        }
    }

    virtual ~Graph()
    {
        for (int i = 0; i < numNodes; i++)
        {
            delete(adjacencyArray[i]);
            delete(nodesArray[i]);
        }
        delete[] nodesDegreeSortedArray;
        delete[] adjacencyArray;
        delete[] nodesArray;
    }

    //Copy-constructor
    Graph(const Graph& other)
    {
        numNodes = other.numNodes;
        numEdges = other.numEdges;
        nodesArray = new GraphNode* [numNodes];
        nodesDegreeSortedArray = new GraphNode* [numNodes];
        adjacencyArray = new vector<GraphNode*>* [numNodes];

        for(int i = 0; i < numNodes; i++)
        {
            nodesArray[i] = new GraphNode(*(other.nodesArray[i]));
        }

        for(int i = 0; i < numNodes; i++)
        {
            nodesDegreeSortedArray[i] = nodesArray[other.nodesDegreeSortedArray[i]->GetLabel()];
        }

        for(int i = 0; i < numNodes; i++)
        {
            unsigned int vectorSize = other.adjacencyArray[i]->size();
            vector<GraphNode*>* adjacentNodes = other.adjacencyArray[i];

            for(unsigned int j = 0; j < vectorSize; j++)
            {
                adjacencyArray[i]->push_back(nodesArray[(adjacentNodes->at(j))->GetLabel()]);
            }
        }
    }

    void addArc(int u, int v)
    {
        adjacencyArray[u - 1]->push_back(nodesArray[u - 1]);
        adjacencyArray[v - 1]->push_back(nodesArray[v - 1]);

        //Incrementar el grado de cada nodo
        ++(*(nodesArray[u - 1]));
        ++(*(nodesArray[v - 1]));
    }

    const vector<GraphNode*>* neighbors(int node_label)
    {
        if (node_label > 0 && node_label <= numNodes)
        {
            return adjacencyArray[node_label - 1];
        }
        else
        {
            throw string("Etiqueta de nodo invalida en Graph::Neighbors");
        }
    }

    void setColor(int node_label, int color)
    {
        if (node_label > 0 && node_label <= numNodes)
        {
            nodesArray[node_label - 1]->SetColor(color);
        }
        else
        {
            throw string("Etiqueta de nodo invalida en Graph::SetColor");
        }
    }

    int getColor(int node_label)
    {
        return nodesArray[node_label - 1]->GetColor();
    }

    void printCurrentColoring()
    {
        cout << "Vertice\tColor\tGrado\tEtiqueta\n";
        for (int i = 0; i < numNodes; i++)
        {
            cout << i + 1 << "\t"
                 << nodesDegreeSortedArray[i]->GetColor() << "\t"
                 << nodesDegreeSortedArray[i]->GetDegree() << "\t"
                 << nodesDegreeSortedArray[i]->GetLabel() << "\n";
        }
    }

    void Dsatur(int tmax)
    {

    }

    void Brown(int tmax)
    {

    }

private:

    vector<GraphNode*> ** adjacencyArray;
    GraphNode** nodesArray;
    GraphNode** nodesDegreeSortedArray;
    int numNodes;
    int numEdges;

    void sortAdjacentNodes()
    {
    }

    void sortNodesByDegree()
    {
        CompareDegreeFunctor comparer;
        sort(nodesDegreeSortedArray, nodesDegreeSortedArray + numNodes,  comparer);
    }

    void initializeNodesArrays()
    {
        for (int i = 0; i < numNodes; i++)
        {
            nodesArray[i] = nodesDegreeSortedArray[i] = new GraphNode();
            nodesArray[i]->SetLabel(i + 1);
        }
    }

    void initializeAdjancencyArray()
    {
        for (int i = 0; i < numNodes; i++)
        {
            adjacencyArray[i] = new vector<GraphNode*>;
        }
    }
};

#endif
