#ifndef GRAPH_H
#define	GRAPH_H

#include <list>
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <ctime>
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
        numColored = 0;
        string linea;
        ifstream instancia(path);
        char* token;
        bool primeraLineaLeida = false;

        if (!instancia.is_open())
            throw string("No se pudo abrir el archivo");

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
        uncoloredNodes = new list<GraphNode*> (nodesArray, nodesArray + numNodes);
        sortNodesByDegree();
        sortAdjacentNodes();
    }


    virtual ~Graph()
    {
        for (int i = 0; i < numNodes; i++)
        {
            delete(adjacencyArray[i]);
            delete(nodesArray[i]);
        }
        delete uncoloredNodes;
        delete[] nodesDegreeSortedArray;
        delete[] adjacencyArray;
        delete[] nodesArray;
    }

    //Copy-constructor
    Graph(const Graph& other)
    {
        numNodes = other.numNodes;
        numEdges = other.numEdges;
        numColored = other.numColored;
        nodesArray = new GraphNode* [numNodes];
        nodesDegreeSortedArray = new GraphNode* [numNodes];
        adjacencyArray = new vector<GraphNode*>* [numNodes];

        for(int i = 0; i < numNodes; i++)
        {
            nodesArray[i] = new GraphNode(*(other.nodesArray[i]));
        }

        for(int i = 0; i < numNodes; i++)
        {
            nodesDegreeSortedArray[i] = nodesArray[other.nodesDegreeSortedArray[i]->GetLabel() - 1];
        }

        for(int i = 0; i < numNodes; i++)
        {
            unsigned int vectorSize = other.adjacencyArray[i]->size();
            vector<GraphNode*>* adjacentNodes = other.adjacencyArray[i];

            for(unsigned int j = 0; j < vectorSize; j++)
            {
                adjacencyArray[i]->push_back(nodesArray[(adjacentNodes->at(j))->GetLabel() - 1]);
            }
        }
    }

    void addArc(int u, int v)
    {
        (adjacencyArray[u - 1])->push_back(nodesArray[v - 1]);
        (adjacencyArray[v - 1])->push_back(nodesArray[u - 1]);

        //Incrementar el grado de cada nodo
        ++(*(nodesArray[u - 1]));
        ++(*(nodesArray[v - 1]));
    }

    const vector<GraphNode*>* neighbors(int node_label)
    {
        if (node_label <= 0 || node_label > numNodes)
        {
            throw string("Etiqueta de nodo invalida en Graph::Neighbors");
        }
        return adjacencyArray[node_label - 1];
    }

    void setColor(int node_label, int color)
    {
        if (node_label <= 0 || node_label > numNodes)
        {
            throw string("Etiqueta de nodo invalida en Graph::SetColor");
        }
        nodesArray[node_label - 1]->SetColor(color);
    }

    void setColorDsatur(int node_label, int color)
    {
        if (node_label > 0 && node_label <= numNodes)
        {
            nodesArray[node_label - 1]->SetColor(color);
            numColored++;
            bool encontrado = false;

            for (list<GraphNode*>::iterator it = uncoloredNodes->begin(); it != uncoloredNodes->end() && !encontrado; it++)
            {
                if((*it)->GetLabel() == node_label)
                {
                    uncoloredNodes->erase(it);
                    encontrado = true;
                }
            }
        }
        else
        {
            throw string("Etiqueta de nodo invalida en Graph::SetColor");
        }

        const vector<GraphNode*> *adjacents = neighbors(node_label);

        for(unsigned int i=0; i< adjacents->size(); i++)
        {
            const vector<GraphNode*> *adjacentNeighbors = neighbors((*adjacents)[i]->GetLabel());
            bool found=false;
            for(unsigned int j=0; j< adjacentNeighbors->size(); j++)
            {
                if((*adjacentNeighbors)[j]->GetLabel() != node_label && (*adjacentNeighbors)[j]->GetColor() == getColor(node_label) && !found)
                {
                    found = true;
                }
            }
            if(!found)
            {
                nodesArray[(*adjacents)[i]->GetLabel() - 1]->IncrementSaturationDegree();
            }
        }
    }

    int getColor(int node_label)
    {
        if (node_label <= 0 || node_label > numNodes)
        {
            throw string("Etiqueta de nodo invalida en Graph::getColor");
        }
        return nodesArray[node_label - 1]->GetColor();
    }

    void printOutput(ostream& output, double time)
    {
        output.precision(8);
        output << "Tiempo de ejecucion: " << fixed << time << "\n";
        output << "Numero de colores encontrados: " << getNumberOfColors() << "\n";
        output << "Vertice\tColor\n";
        for (int i = 0; i < numNodes; i++)
        {
            output << i + 1 << "\t" << nodesArray[i]->GetColor() << "\n";
        }
    }

    void printCurrentColoring()
    {
        cout << "Vertice\tColor\n";
        for (int i = 0; i < numNodes; i++)
        {
            cout << i + 1 << "\t"
                 << nodesArray[i]->GetColor() << "\n";
        }
    }

    void printGraphStructure()
    {
        cout << "Estructura del grafo:" << endl;
        vector<GraphNode*> * adjacents;
        for (int i = 0; i < numNodes; i++)
        {
            cout << "Nodo " << i + 1 << " es adyacente a:" <<endl;
            adjacents = adjacencyArray[i];
            for (unsigned int j = 0; j < adjacents->size(); j++)
            {
                cout << "Etiqueta: " << (*adjacents)[j]->GetLabel() << " Color:" << (*adjacents)[j]->GetColor() << endl;
            }
            cout << "\n";
        }
    }

    const vector<GraphNode*>* MaximalSaturationDegree()
    {
        vector<GraphNode*>* maximalSaturation = new vector<GraphNode*>;
        int max = 0;

        for (list<GraphNode*>::iterator it = uncoloredNodes->begin(); it != uncoloredNodes->end(); it++)
        {
            if((*it)->GetSaturationDegree() > max)
            {
                max = (*it)->GetSaturationDegree();
            }
        }
        for (list<GraphNode*>::iterator it = uncoloredNodes->begin(); it != uncoloredNodes->end(); it++)
        {
            if((*it)->GetSaturationDegree() == max)
            {
                maximalSaturation->push_back(*it);
            }
        }
        return maximalSaturation;
    }

    int getNumberOfColors()
    {
        int max=0;
        for(int i=0; i<numNodes; i++)
        {
            if(nodesArray[i]->GetColor()>max)
            {
                max = nodesArray[i]->GetColor();
            }
        }
        return max;
    }

    double Dsatur(int tmax)
    {
        time_t insideTime;
        time_t startTime;
        time_t endTime;
        clock_t startTime2 = clock();
        clock_t endTime2;

        time(&startTime);
        cout << startTime2 << "\n";
        setColorDsatur(nodesDegreeSortedArray[0]->GetLabel(), 1);
        while(numColored < numNodes)
        {
            const vector<GraphNode*>* maximalSaturation = MaximalSaturationDegree();
            if(maximalSaturation->size()==1)
            {
                int label = maximalSaturation->front()->GetLabel();
                int minColor = GetMinimumFeasibleColor(label);
                setColorDsatur(label, minColor);
            }
            else
            {
                int maxDegreeLabel;
                int maxDegree = 0;
                for(unsigned int i=0; i< maximalSaturation->size(); i++)
                {
                    if((*maximalSaturation)[i]->GetDegree() > maxDegree)
                    {
                        maxDegree = (*maximalSaturation)[i]->GetDegree();
                        maxDegreeLabel= (*maximalSaturation)[i]->GetLabel();
                    }
                }
                int minColor = GetMinimumFeasibleColor(maxDegreeLabel);
                setColorDsatur(maxDegreeLabel, minColor);
            }
            time(&insideTime);
            if(difftime(insideTime,startTime)>= tmax)
            {
                throw string("Tiempo de ejecucion excedido");
            }
        }

        time(&endTime);
        endTime2 = clock();
        cout << endTime2 << "\n";
        return (double)(endTime2 - startTime2)/(double)CLOCKS_PER_SEC;
    }

    void Brown(int tmax)
    {
        asignarColoracionInicialBrown();
    }

private:

    vector<GraphNode*> **adjacencyArray;
    GraphNode** nodesArray;
    GraphNode** nodesDegreeSortedArray;
    int numNodes;
    int numColored;
    int numEdges;
    list<GraphNode*> * uncoloredNodes;

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

    int GetMinimumFeasibleColor(int node_label)
    {
        if (node_label <= 0 || node_label > numNodes)
        {
            throw string("Etiqueta de nodo invalida en Graph::GetMinimumFeasibleColor");
        }

        bool adjacentColors[numNodes];
        bool * colorMinimo;
        int color = 0;

        for(int i = 0; i < numNodes; i++)
            adjacentColors[i] = false;

        vector<GraphNode*> * adjacentNodes = adjacencyArray[node_label - 1];

        for(unsigned int i = 0; i < adjacentNodes->size(); i++)
        {
            color = ((*adjacentNodes)[i])->GetColor();
            if (color > 0)
                adjacentColors[color - 1] = true;
        }

        if ((colorMinimo = find(adjacentColors, adjacentColors + numNodes, false))
                < adjacentColors + numNodes)
            return colorMinimo - adjacentColors + 1;
        else
            return 0;
    }

    void asignarColoracionInicialBrown()
    {
        int color;

        for(int i = 0; i < numNodes; i++)
        {
            if ((color = GetMinimumFeasibleColor(i + 1)) == 0)
                throw string("No se pudo completar la coloracion inicial");
            nodesArray[i]->SetColor(color);
        }
    }
};

#endif
