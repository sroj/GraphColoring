#ifndef COMPAREDEGREEFUNCTOR_H
#define	COMPAREDEGREEFUNCTOR_H

#include "GraphNode.h"

class CompareDegreeFunctor {
public:
    bool operator() (const GraphNode* node_1, const GraphNode* node_2) {
        return (node_1->GetDegree())>(node_2->GetDegree());
    }
};

#endif	/* COMPAREDEGREEFUNCTOR_H */

