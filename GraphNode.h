#ifndef GRAPHNODE_H
#define	GRAPHNODE_H

class GraphNode
{
public:

    GraphNode() : color(0), saturationDegree(0), degree(0), label(0)
    {
    };

    //Copy constructor
    GraphNode(const GraphNode& orig)
    {
        color = orig.color;
        saturationDegree = orig.saturationDegree;
        degree = orig.degree;
        label = orig.label;
    };

    virtual ~GraphNode()
    {
        //Nada interesante que hacer aqui
    };

    int GetSaturationDegree() const
    {
        return saturationDegree;
    }

    void SetSaturationDegree(int SaturationDegree)
    {
        this->saturationDegree = SaturationDegree;
    }

    void IncrementSaturationDegree()
    {
        this->saturationDegree++;
    }

    int GetColor() const
    {
        return color;
    }

    void SetColor(int color)
    {
        this->color = color;
    }

    int GetDegree() const
    {
        return degree;
    }

    void SetDegree(int degree)
    {
        this->degree = degree;
    }

    int GetLabel() const
    {
        return label;
    }

    void SetLabel(int label)
    {
        this->label = label;
    }

    GraphNode& operator++()
    {
        ++degree;
        return *this;
    }

private:
    int color;
    int saturationDegree;
    int degree;
    int label;
};

#endif	/* GRAPHNODE_H */

