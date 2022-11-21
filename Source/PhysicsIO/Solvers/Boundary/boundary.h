#ifndef _PHYSICSIO_BOUNDARY_
#define _PHYSICSIO_BOUNDARY_

#define BOUNDARY_CONDITION_NONE        0
#define BOUNDARY_CONDITION_REFLECTING  1
#define BOUNDARY_CONDITION_DIRICHLET   2

#include "../solvers.h"

class physicsio::Boundary
{
public:
    int boundary_type;
};
#endif