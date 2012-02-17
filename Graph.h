#include <list>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class Graph
{
public:
    Graph(const char path[])
    {
        int u;
        int v;
        string linea;
        ifstream instancia (path);
        char* token;

        if (instancia.is_open())
        {
            while (instancia.good())
            {
                getline (instancia,linea);
                char copia [linea.size() + 1];
                strcpy(copia, linea.c_str());

                token = strtok (copia," ");
                while (token!=NULL)
                {
                    if(strcmp(token, "e"))
                    {
                        token = strtok(NULL," ");
                        u = atoi(token);
                        token = strtok(NULL," ");
                        v = atoi(token);
                        addArc(u,v);
                    }
                    else if(strcmp(token,"c")==0)
                    {
                        continue;
                    }
                    else if(strcmp(token, "p")==0)
                    {
                        token=strtok(NULL," ");
                        token=strtok(NULL," ");

                        n = atoi(token);

                        token=strtok(NULL," ");

                        m = atoi(token);

                        adjacencyArray = new list<int>* [n];

                        nodes = new int[n];
                    }
                    token=strtok(NULL," ");
                }
            }
            instancia.close();
        }
        else
        {
            //throw new exception("No se pudo abrir el archivo");
        }
    }
    virtual ~Graph() {}

    Graph(const Graph& other)
    {
    }

    void addArc(int u, int v)
    {
        adjacencyArray[u]->push_front(v);
        adjacencyArray[v]->push_front(u);
    }

    list<int>* neighbors(int node_label)
    {
        return new list<int> (*(adjacencyArray[node_label]));
    }

    void setColor(int node_label, int color)
    {
        if(node_label>0 && node_label <= n)
        {
            nodes[node_label-1] = color;
        }
        else
        {
            throw new exception();
        }
    }

    int getColor(int node_label)
    {
        return nodes[node_label-1];
    }

    int* currentColoring()
    {

    }

    void Dsatur(int tmax)
    {

    }
    void Brown(int tmax)
    {

    }

private:

    list<int>** adjacencyArray;
    int* nodes;
    int n;
    int m;
};
