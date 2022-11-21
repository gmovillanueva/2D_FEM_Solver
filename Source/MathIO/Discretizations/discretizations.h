#ifndef _MATHIO_DISCRETIZATIONS_
#define _MATHIO_DISCRETIZATIONS_

#include "../mathio.h"
#include "../../MeshIO/meshio.h"

class mathio::Discretizations
{
public:
    int dim;

public:


    Discretizations(int dim);

    virtual void AddViewOfContinuum(meshio::MeshContinuum* volume_continuum);
    virtual void AddViewOfLocalContinuum(meshio::MeshContinuum* volume_continuum,
                                         int number_cells,
                                         int* cell_indices);
};

#endif // DISCRETIZATIONS_H