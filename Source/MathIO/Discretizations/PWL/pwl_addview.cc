#include "pwl.h"

#include "pwl_triangle.h"
#include "pwl_polygon.h"



void mathio::PWL::AddViewOfContinuum(meshio::MeshContinuum *volume_continuum)
{
    std::vector<meshio::Cell*>::iterator cell;
    for(cell = volume_continuum->cells.begin(); cell != volume_continuum->cells.end(); cell++)
    {
        if(typeid(*(*cell)) == typeid(meshio::CellTriangle))
        {
            auto* view = new TriangleFEView((meshio::CellTriangle*)(*cell), volume_continuum);
            this->cell_face_views.push_back(view);
        }
        if(typeid(*(*cell)) == typeid(meshio::CellPolygon))
        {
            auto* view = new PolygonFEView((meshio::CellPolygon*)(*cell), volume_continuum);
            this->cell_face_views.push_back(view);
        }
    }
}


void mathio::PWL::AddViewOfLocalContinuum(meshio::MeshContinuum *volume_continuum, int number_cells, int *cell_indices)
{
    if(!mapping_initialized)
    {
        this->cell_face_views_mapping.reserve(volume_continuum->cells.size());
        std::vector<meshio::Cell*>::iterator cell_iterator;
        for(cell_iterator = volume_continuum->cells.begin(); cell_iterator != volume_continuum->cells.end(); cell_iterator++)
        {
            this->cell_face_views_mapping.push_back(-1);
        }
        mapping_initialized = true;
    }

    int cell_index;
    for(int c = 0; c < number_cells; c++)
    {
        cell_index = cell_indices[c];
        auto* cell = volume_continuum->cells[cell_index];

        if(cell_face_views_mapping[cell_index] < 0)
        {
            if(typeid((*cell)) == typeid(meshio::CellTriangle))
            {
                auto* view = new TriangleFEView((meshio::CellTriangle*)(cell), volume_continuum);
                this->cell_face_views.push_back(view);
                cell_face_views_mapping[cell_index] = (int)this->cell_face_views.size()-1;
            }
            if(typeid((*cell)) == typeid(meshio::CellPolygon))
            {
                auto* view = new PolygonFEView((meshio::CellPolygon*)(cell), volume_continuum);
                this->cell_face_views.push_back(view);
                cell_face_views_mapping[cell_index] = (int)this->cell_face_views.size()-1;
            }
        }
    }
}


CellFEView* mathio::PWL::MapFeView(int cell_global_index)
{
    CellFEView* value = cell_face_views.at((unsigned long)cell_face_views_mapping[cell_global_index]);
    return value;
}