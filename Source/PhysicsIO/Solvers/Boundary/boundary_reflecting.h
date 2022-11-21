#ifndef _PHYSICSIO_BOUNDARY_REFLECTING_
#define _PHYSICSIO_BOUNDARY_REFLECTING_

#include "boundary.h"

class physicsio::BoundaryReflecting : public physicsio::Boundary
{
public:
    double boundary_value{};

public:
    BoundaryReflecting();
};

physicsio::BoundaryReflecting::BoundaryReflecting() : Boundary()
{
    boundary_type = BOUNDARY_CONDITION_REFLECTING;
}

#endif // BOUNDARY_REFLECTING_H