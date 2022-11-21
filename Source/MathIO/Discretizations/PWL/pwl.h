#ifndef _MATHIO_PWL_
#define _MATHIO_PWL_

#include "../discretizations.h"
#include "pwl_base.h"

#include <vector>
#include <typeinfo>


class mathio::PWL : public mathio::Discretizations
{
public:
    bool mapping_initialized;

    std::vector<CellFEView*> cell_face_views;
    std::vector<int> cell_face_views_mapping;

public:
    void AddViewOfContinuum(meshio::MeshContinuum* volume_continuum) override;
    void AddViewOfLocalContinuum(meshio::MeshContinuum* volume_continuum,
                                         int number_cells,
                                         int* cell_indices) override;
    CellFEView* MapFeView(int cell_global_index);

};

#endif // PWL_H