#include "diffusion2d.h"

#include "../../../MeshIO/MeshHandler/meshhandler.h"
#include "../Boundary/boundary_dirichlet.h"



extern std::vector<meshio::MeshHandler*> meshhandler_stack;


void physicsio::Diffusion2D::InitializeItems()
{
    meshio::Region* region = this->regions.back();
    //meshio::MeshHandler* mesh_handler = meshio::GetCurrentHandler();
    //meshio::VolumeMesher* mesher = mesh_handler->volume_mesher;

    for(int b = 0; b < region->boundaries.size(); b++)
    {
        auto* new_boundary = new physicsio::BoundaryDirichlet;
        this->boundaries.push_back(new_boundary);
    }

    initialize_items = true;
}