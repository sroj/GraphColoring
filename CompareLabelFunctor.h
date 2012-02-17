#ifndef COMPARELABELFUNCTOR_H
#define	COMPARELABELFUNCTOR_H

#include "GraphNode.h"

class CompareLabelFunctor {
public:
    bool operator() (const GraphNode* node_1, const GraphNode* node_2) {
        return (node_1->GetLabel())<(node_2->GetLabel());
    }
};


#endif	/* COMPARELABELFUNCTOR_H */

