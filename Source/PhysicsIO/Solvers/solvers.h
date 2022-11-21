#ifndef _PHYSICSIO_SOLVERS_
#define _PHYSICSIO_SOLVERS_

#include "../physicsio.h"
#include "../../MeshIO/Region/region.h"



class physicsio::Solvers
{
public:
    std::vector<meshio::Region*> regions;
};

#endif // SOLVERS_H