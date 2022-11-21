#ifndef _PHYISCSIO_BOUNDARY_DIRICHLET_
#define _PHYISCSIO_BOUNDARY_DIRICHLET_

#include "boundary.h"

class physicsio::BoundaryDirichlet : public physicsio::Boundary
{
public:
    double boundary_value;

public:
    BoundaryDirichlet() : Boundary()
    {
        boundary_type = BOUNDARY_CONDITION_DIRICHLET;
        boundary_value = 0.0;
    }
};

#endif // BOUNDARY_DIRICHLET_H